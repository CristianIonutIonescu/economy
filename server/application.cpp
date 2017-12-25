#include "application.hpp"
#include <iostream> // to remove
#include <chrono>

namespace economy
{
namespace server
{
Application::Application(int argc, char **argv)
{
    if (argc < 3)
    {
        throw std::runtime_error("Server requires 3 arguments");
    }

    std::string address = argv[1];
    std::string path = argv[2];
    data_sync_ = std::make_unique<std::mutex>();

    StartParser(path);
    StartServer(address);
}

void Application::StartParser(const std::string &file_path)
{
    parser_ = std::make_unique<DataParser>(file_path, data_sync_.get());
    parser_thread_ = std::thread([this]() {
        using namespace std::chrono_literals;
        while (!parser_->Closing()) 
        {
            if(parser_->CheckDataFresh()) {
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
    std::cout<<"Starting server..." << std::endl;
    server_ = builder.BuildAndStart();
    server_->Wait();
}

void Application::Run()
{
}

void Application::Close() {
    server_->Shutdown();
    server_thread_.join();

    parser_->Shutdown();
    parser_thread_.join();
}
} // namespace server
} // namespace economy