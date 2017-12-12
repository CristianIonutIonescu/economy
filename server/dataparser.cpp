#include "dateparser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace economy
{
namespace server
{
void DataParser::Init(const std::string &path)
{
    std::ifstream file(path);
    std::string line;
    while(std::getline(file, line)) {
        size_t separator_pos = line.find(separator);
        economy::Date date;
        date.FromString(line.substr(0, separator_pos), date_format);
        float value = std::stof(line.substr(separator_pos+1));
        if(data.find(date) != data.end()) {
            data[date] +=value;
        } else {
            data.insert({date,value});
        }
    }
}
std::vector<std::pair<economy::Date, float>> DataParser::GetRange(const economy::Date &beg, const economy::Date &end) const
{
    std::vector<std::pair<economy::Date, float>> range;
    for (const auto &it : data)
    {
        if (it.first >= beg && it.first <= end)
        {
            range.push_back(it);
        }
    }

    return range;
}

void DataParser::Print() const {
    for(const auto &it: data) {
        std::cout<<it.first.ToString(date_format)<<separator<<it.second<<std::endl;
    }
}
}
}
