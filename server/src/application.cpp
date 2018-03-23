#include "application.hpp"

#include "../../common/utils.hpp"

#include <iostream> // to remove
#include <chrono>
#include <ctime>
#include <gflags/gflags.h>

static bool ValidatePort(const char *flagname, int32_t value)
{
    if (value > 0 && value < 32768) // value is ok
        return true;
    std::cerr<<"Invalid value for --"<< flagname<< ":" <<value<<std::endl;
    return false;
}

DEFINE_string(address, "localhost", "server listening address");
DEFINE_int32(port, 8080, "server listening port");
DEFINE_validator(port, &ValidatePort);
DEFINE_string(savings_path, "./resources/data.csv", "savings file path");
DEFINE_string(currencies_path, "./resources/exchanges.csv", "exchange rates file path");
DEFINE_string(script_path, "./scripts/currencyretriever.py", "exchange rates retriever path");

namespace economy
{
namespace server
{

volatile bool Application::s_close = false;

Application::Application(int argc, char **argv)
{
    ::google::ParseCommandLineFlags(&argc, &argv, true);
    data_sync_ = std::make_unique<std::mutex>();
    StartParser(FLAGS_savings_path);
    StartRetriever(FLAGS_currencies_path, FLAGS_script_path);
    StartServer(FLAGS_address);
}

void Application::StartParser(const std::string &file_path)
{
    parser_ = std::make_unique<DataParser>(file_path, data_sync_.get());
    parser_thread_ = std::thread([this]() {
        using namespace std::chrono_literals;
        while (!parser_->Closing())
        {
            if (parser_->CheckDataFresh())
            {
                std::cout << "Updating data..." << std::endl;
                parser_->ReadData();
            }

            std::this_thread::sleep_for(5s);
        }
    });
}

void Application::StartServer(const std::string &address)
{
    grpc::ServerBuilder builder;
    // TODO: move to secure credentials
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    TransportServiceImpl service(parser_.get());
    builder.RegisterService(&service);

    std::cout << "Starting server..." << std::endl;
    server_ = builder.BuildAndStart();

    server_thread_ = std::thread([this, &builder]() {
        server_->Wait();
    });

    int test = 3;
}

void Application::StartRetriever(const std::string &data_path, const std::string &script_path)
{
    currency_retriever_ = std::make_unique<CurrencyRetriever>(data_sync_.get(), data_path, script_path);
    currency_thread_ = std::thread([this]() {
        while (!currency_retriever_->Closing())
        {
            struct tm *tm_time = GetCurrentTime();
            using namespace std::chrono_literals;
            // check if weekend
            if (tm_time->tm_wday > 5)
            {

                std::this_thread::sleep_for(24h);
            }

            if (tm_time->tm_hour < 13)
            {
                std::this_thread::sleep_for(1h);
            }

            bool currency_retrieved = false;
            size_t fails = 0;
            while (!currency_retrieved)
            {
                currency_retrieved = currency_retriever_->RequestCurrency();
                if (!currency_retrieved)
                {
                    std::this_thread::sleep_for(5min);
                    fails++;
                }

                if (fails > 20)
                {
                    std::cerr << "Could not retrieve currency on: "
                              << tm_time->tm_mday << "-" << tm_time->tm_mon
                              << tm_time->tm_year + 1900 << std::endl;
                    break;
                }
            }

            std::this_thread::sleep_for(24h - (fails * 5min));
        }
    });
}

void Application::Run()
{
    using namespace std::chrono_literals;
    while (!s_close)
    {
        std::this_thread::sleep_for(5s);
    }
}

void Application::Close()
{
    server_->Shutdown();
    if (server_thread_.joinable())
    {
        server_thread_.join();
    }

    parser_->Shutdown();
    if (parser_thread_.joinable())
    {
        parser_thread_.join();
    }

    currency_retriever_->Shutdown();
    if (currency_thread_.joinable())
    {
        currency_thread_.join();
    }

    ::google::ShutDownCommandLineFlags();
}
} // namespace server
} // namespace economy