#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "transportservice.hpp"
#include "dataparser.hpp"

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
    ~Application() { Close(); }
    void StartServer(const std::string &address);
    void StartParser(const std::string &file_path_);
    void Run();
    void Close();
  private:

    std::unique_ptr<DataParser> parser_;
    std::unique_ptr<grpc::Server> server_;
    std::unique_ptr<std::mutex> data_sync_;
    std::thread parser_thread_;
    std::thread server_thread_;
};
}
}
#endif