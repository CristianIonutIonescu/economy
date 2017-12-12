#ifndef TRANSPORTSERVICE_HPP
#define TRANSPORTSERVICE_HPP

#include <grpc++/grpc++.h>
#include "datatypes.grpc.pb.h"

namespace economy
{
namespace server
{
class TransportServiceImpl final : public TransportService::Service
{
    grpc::Status GetData(grpc::ServerContext *context,
                         const DataRequest *request, 
                         grpc::ServerWriter<DataReply> *writer) {}

    grpc::Status ChangeCurrency(grpc::ServerContext *context,
                                const CurrencyRequest *request, 
                                CurrencyReply *response) {}
};
}
}

#endif