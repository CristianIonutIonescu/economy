#ifndef DATEPARSER_HPP
#define DATEPARSER_HPP

#include "../../common/date.hpp"

#include <atomic>
#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <time.h>

namespace economy
{
namespace server
{

using data_container = std::vector<std::pair<economy::Date, float>>;
class DataParser
{
  public:
    explicit DataParser(const std::string &path, std::mutex *data_lock)
        : path_(path),
          data_lock_(data_lock),
          last_read_date_() {}

    DataParser(const DataParser &) = delete;
    DataParser(const DataParser &&) = delete;

    DataParser operator=(const DataParser &) = delete;
    DataParser operator=(const DataParser &&) = delete;

    void ReadData();

    data_container GetRange(const economy::Date &beg, 
                            const economy::Date &end) const;

    inline void Shutdown() noexcept { stop_ = true; }

    inline bool Closing() const noexcept { return stop_; }

    bool CheckDataFresh() const;

  private:
    static const char separator = ',';
    static constexpr auto date_format = "DD-MM-YYYY";
    static const uint64_t day_in_seconds = 60 * 60;

    std::string path_;
    time_t last_read_date_;
    std::map<economy::Date, float> data_;
    std::mutex *data_lock_;
    bool stop_;
};
}
}
#endif