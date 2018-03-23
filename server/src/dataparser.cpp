#include "dataparser.hpp"

#include "../../common/utils.hpp"

#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>

namespace economy
{
namespace server
{

using data_container = std::vector<std::pair<economy::Date, float>>;

void DataParser::ReadData()
{
    std::lock_guard<std::mutex> lock(*data_lock_);
    std::ifstream file(path_);

    auto backup_data = data_;
    data_.clear();

    std::string line;

    try
    {
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                break;
            }

            auto content = ParseCSVLine(line, separator);
            economy::Date date(content[0], date_format);

            float value = std::stof(content[1]);

            if (data_.find(date) != data_.end())
            {
                data_[date] += value;
            }
            else
            {
                data_.insert({date, value});
            }
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Could not read data: " << ex.what()
                  << ". Old data in place." << std::endl;
        data_ = backup_data;
    }

    last_read_date_ = time(0);
    file.close();
}
data_container DataParser::GetRange(const economy::Date &beg,
                                    const economy::Date &end) const
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
}
}
