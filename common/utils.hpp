#ifndef UTILS_HPP
#define UTILS_HPP

#include "date.hpp"

struct tm;

namespace economy
{

CurrencyType CurrencyTypeFromString(const std::string &str);

std::string CurrencyTypeToString(const CurrencyType type);

std::vector<std::string> ParseCSVLine(const std::string &line, const char sep);

struct tm *GetCurrentTime();
}
#endif