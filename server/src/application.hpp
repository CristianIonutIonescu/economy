#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "currencyretriever.hpp"
#include "dataparser.hpp"
#include "transportservice.hpp"

#include <memory.h>
#include <thread>

namespace economy
{
namespace server
{

class Application
{
  public:
    explicit Application(int argc, char **argv);
    ~Application() = default;
    void StartServer(const std::string &address);
    void StartParser(const std::string &file_path_);
    void StartRetriever(const std::string &data_path, const std::string &script_path);
    void Run();
    void Close();

    static volatile bool s_close;
  private:

    std::unique_ptr<DataParser> parser_;
    std::unique_ptr<grpc::Server> server_;
    std::unique_ptr<CurrencyRetriever> currency_retriever_;

    std::unique_ptr<std::mutex> data_sync_;
    std::thread parser_thread_;
    std::thread server_thread_;
    std::thread currency_thread_;
};
}
}
#endif