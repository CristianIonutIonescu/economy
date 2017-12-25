#include "dataparser.hpp"

#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>

namespace economy
{
namespace server
{
void DataParser::ReadData()
{
    std::lock_guard<std::mutex> lock(*data_lock_);

    std::ifstream file(path_);
    std::string line;
    while (std::getline(file, line))
    {
        if(line.empty()){
            break;
        }
        
        size_t separator_pos = line.find(separator);
        economy::Date date;
        auto str_date = line.substr(0, separator_pos);
        date.FromString(str_date, date_format);
        float value = std::stof(line.substr(separator_pos + 1));
        if (data_.find(date) != data_.end())
        {
            data_[date] += value;
        }
        else
        {
            data_.insert({date, value});
        }
    }
    last_read_date_ = time(0);
    file.close();
}
std::vector<std::pair<economy::Date, float>> DataParser::GetRange(const economy::Date &beg, const economy::Date &end) const
{
    std::lock_guard<std::mutex> lock(*data_lock_);
    std::vector<std::pair<economy::Date, float>> range;
    //TODO: optimize iteration
    for (const auto &it : data_)
    {
        if (it.first >= beg && it.first <= end)
        {
            range.push_back(it);
        }
    }

    return range;
}

bool DataParser::CheckDataFresh() const
{
    struct stat result;
    struct timespec last_modified_date;

    if (stat(path_.c_str(), &result) == 0)
    {
        last_modified_date = result.st_mtim;
    }

    return last_modified_date.tv_sec > last_read_date_;
}

void DataParser::Print() const
{
    for (const auto &it : data_)
    {
        std::cout << it.first.ToString(date_format) << separator << it.second << std::endl;
    }
}
}
}
