#ifndef TRANSPORTSERVICE_HPP
#define TRANSPORTSERVICE_HPP

#include <grpcpp/grpcpp.h>
#include "../../common/datatypes.grpc.pb.h"
#include "dataparser.hpp"

namespace economy
{
namespace server
{
class TransportServiceImpl final : public TransportService::Service
{
  public:
    explicit TransportServiceImpl(DataParser *parser): parser_(parser) {}
    
    grpc::Status GetData(grpc::ServerContext *context,
                         const DataRequest *request,
                         DataReply *reply) override;

    grpc::Status ChangeCurrency(grpc::ServerContext *context,
                                const CurrencyRequest *request,
                                CurrencyReply *response) override;

  private:
    DataParser *parser_;
};
}
}

#endif