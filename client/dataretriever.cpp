#include "dataretriever.hpp"
#include <grpc++/grpc++.h>

namespace economy
{
namespace client
{
DataRetriever::DataRetriever(const std::string &address)
{
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
    stub_ = TransportService::NewStub(channel);
}

std::vector<std::pair<Date, float>> DataRetriever::GetData(const Date &begin, const Date &end)
{
    // build request

    ProtoDate proto_beg = begin.ToProtoDate();
    ProtoDate proto_end = end.ToProtoDate();
    DataRequest request;

    std::cout<<"preparing request"<<std::endl;
    request.mutable_begin()->CopyFrom(proto_beg);
    request.mutable_end()->CopyFrom(proto_end);

    grpc::ClientContext context;

    std::cout<<"preparing reply"<<std::endl;
    DataReply reply;
    grpc::Status status = stub_->GetData(&context, request, &reply);
    std::vector<std::pair<Date, float>> data;

    for(const auto it: reply.content()) {
        Date date;
        date.FromProtoDate(it.date());
        float value = it.value();
        data.push_back(std::make_pair(date,value));
    }

    for(const auto it: data) {
        std::cout<<it.first.ToString() << " , " << it.second<<std::endl;
    }

    if(!status.ok()) {
        std::cerr<<status.error_code()<<std::endl;
        throw std::runtime_error(status.error_message());
    }

    std::cout<<"preparing reply 3"<<std::endl;
    return data;
}

double DataRetriever::ChangeCurrency(const Currency &currency)
{
    CurrencyRequest request;
    request.set_new_currency(currency);
    CurrencyReply reply;
    grpc::ClientContext context;
    grpc::Status status = stub_->ChangeCurrency(&context, request, &reply);

    if(!status.ok()) {
        std::cerr<<status.error_code()<<std::endl;
        throw std::runtime_error(status.error_message());
    }

    return reply.conversion_rate();
}
}
}