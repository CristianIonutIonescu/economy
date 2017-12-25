#include "transportservice.hpp"
#include "dataparser.hpp"

namespace economy
{
namespace server
{

namespace
{
const size_t kDataPerResponse = 30;
}

grpc::Status TransportServiceImpl::GetData(grpc::ServerContext *context,
                                           const DataRequest *request,
                                          DataReply *reply)
{
    std::cout<<"Data request in progress.."<<std::endl;
    const auto beg = Date(request->begin());
    const auto end = Date(request->end());
    std::cout<<"Request data from: " << beg.ToString() << " to: "<<end.ToString()<<std::endl;

    const auto data = parser_->GetRange(Date(request->begin()), Date(request->end()));
    // keep this for using streaming
    const size_t data_per_responce = data.size() > kDataPerResponse ? kDataPerResponse : data.size();

        // send a chunck
    for(int i = 0 ; i< data.size(); ++i) {
        DataPayload *payload = reply->add_content();
        auto day_data = data[i];
        ProtoDate proto_date = day_data.first.ToProtoDate();
        payload->mutable_date()->CopyFrom(proto_date);
        payload->set_value(day_data.second);
    }

    return grpc::Status::OK;
}


grpc::Status TransportServiceImpl::ChangeCurrency(grpc::ServerContext *context,
                                                  const CurrencyRequest *request,
                                                  CurrencyReply *response)
{
    response->set_conversion_rate(1.5);
    return grpc::Status::OK;
}
} // namespace server
} // namespace economy