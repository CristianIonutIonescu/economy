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
    //const size_t data_per_responce = data.size() > kDataPerResponse ? kDataPerResponse : data.size();

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
    Currency new_currency;
    new_currency.set_type(request->new_currency());

    std::cout<<"Currency request in progress.."<<std::endl;
    switch(request->new_currency())
    {
        case CurrencyType::RON:
        new_currency.set_conversion_rate(1.0);
        break;
        case CurrencyType::EUR:
        std::cout<<"Changing to EURO"<<std::endl;
        new_currency.set_conversion_rate(4);
        break;
        case CurrencyType::USD:
        std::cout<<"Changing to USD"<<std::endl;
        new_currency.set_conversion_rate(3);
        break;

    }
    response->mutable_currency()->CopyFrom(new_currency);
    return grpc::Status::OK;
}
} // namespace server
} // namespace economy