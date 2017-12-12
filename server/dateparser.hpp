#ifndef DATEPARSER_HPP
#define DATEPARSER_HPP

#include "date.hpp"

#include <map>
#include <vector>
#include <string>

namespace economy
{
namespace server
{
class DataParser
{
  public:
    explicit DataParser() = default;

    DataParser(const DataParser &) = delete;
    DataParser(const DataParser &&) = delete;

    DataParser operator=(const DataParser &) = delete;
    DataParser operator=(const DataParser &&) = delete;

    void Init(const std::string &path);
    std::vector<std::pair<economy::Date, float>> GetRange(const economy::Date &beg, const economy::Date &end) const;
    void Print() const;
  private:
    std::map<economy::Date, float> data;
    static const char separator = ',';
    static constexpr auto date_format  = "DD-MM-YYYY";
};
}
}
#endif