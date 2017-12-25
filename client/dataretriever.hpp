#ifndef DATARETRIEVER_HPP
#define DATARETRIEVER_HPP

#include <string>
#include <vector>
#include <memory>

#include "../common/date.hpp"
#include "../common/datatypes.grpc.pb.h"

namespace economy
{
namespace client
{

class DataRetriever
{
  public:
    explicit DataRetriever(const std::string &address);
    DataRetriever(const DataRetriever &) = delete;
    DataRetriever operator=(const DataRetriever &) = delete;

    std::vector<std::pair<Date, float>> GetData(const Date &begin, const Date &end);
    double ChangeCurrency(const Currency &currency);
    private:
    std::unique_ptr<TransportService::Stub> stub_;
};
} // namespace client
} // namespace economy
#endif