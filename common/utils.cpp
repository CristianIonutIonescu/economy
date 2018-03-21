#include "utils.hpp"

#include <chrono>
#include <ctime>

namespace economy
{

CurrencyType CurrencyTypeFromString(const std::string &str)
{
    if (str == "RON")
    {
        return CurrencyType::RON;
    }
    else if (str == "EUR")
    {
        return CurrencyType::EUR;
    }
    if (str == "USD")
    {
        return CurrencyType::USD;
    }
}

std::string CurrencyTypeToString(const CurrencyType type)
{
    switch (type)
    {
    case CurrencyType::RON:
        return "RON";
    case CurrencyType::EUR:
        return "EUR";
    case CurrencyType::USD:
        return "USD";
    default:
        return "UNIMPLEMENTED";
    }
}

std::vector<std::string> ParseCSVLine(const std::string &line, const char sep)
{
    std::vector<std::string> content;
    size_t prev_pos = 0;
    while (true)
    {
        size_t pos = line.find(sep, prev_pos);
        if (pos == std::string::npos)
        {
            std::string el = line.substr(prev_pos);
            std::cout<<el<<std::endl;
            content.push_back(el);
            break;
        }

        std::string el = line.substr(prev_pos, pos - prev_pos);
        std::cout<<el<<std::endl;
        content.push_back(el);
        prev_pos = pos+1;
    }

    return content;
}

struct tm * GetCurrentTime()
{
    std::chrono::time_point<std::chrono::system_clock> current_time;
    current_time = std::chrono::system_clock::now();
    std::time_t time_t_time = std::chrono::system_clock::to_time_t(current_time);

    struct tm *tm_time = std::localtime(&time_t_time);
}
}