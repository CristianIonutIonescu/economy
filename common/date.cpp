#include "date.hpp"

#include <algorithm>
#include <regex>
#include <stdexcept>

namespace economy
{
std::string Date::ToString(const std::string &format) const
{
    if (std::find(begin(date_formats), end(date_formats), format) != end(date_formats))
    {
        throw std::runtime_error("Format not supported!");
    }

    std::string sdate = format;

    std::string sday = std::to_string(day_);
    if (day_ < 10)
    {
        sday += "0";
    }

    std::string smonth = std::to_string(month_);
    if (month_ < 10)
    {
        smonth += "0";
    }

    sdate = sdate.replace(sdate.find("DD"), 2, sday);
    sdate = sdate.replace(sdate.find("MM"), 2, smonth);
    sdate = sdate.replace(sdate.find("YYYY"), 4, std::to_string(year_));
    return sdate;
}

void Date::FromString(const std::string &date, const std::string &format)
{
    std::regex date_regex("\\b\\d+\\b");
    auto numbers_begin = std::sregex_iterator(date.begin(), date.end(), date_regex);
    auto numbers_end = std::sregex_iterator();

    std::vector<std::string> values;
    for (auto &iter = numbers_begin; iter != numbers_end; ++iter)
    {
        std::smatch match = *iter;
        values.push_back(match.str());
    }

    std::string temp_format = format;
    const std::string separator = format.find("-") != std::string::npos ? "-" : "/";
    temp_format = temp_format.replace(temp_format.find(separator), 1, "");
    temp_format = temp_format.replace(temp_format.find(separator), 1, "");

    if (temp_format == "DDMMYYYY")
    {
        day_ = std::stoi(values[0]);
        month_ = std::stoi(values[1]);
        year_ = std::stoi(values[2]);
    }
    else if (temp_format == "MMDDYYYY")
    {
        month_ = std::stoi(values[0]);
        day_ = std::stoi(values[1]);
        year_ = std::stoi(values[2]);
    }
    else
    {
        year_ = std::stoi(values[0]);
        month_ = std::stoi(values[1]);
        day_ = std::stoi(values[2]);
    }
}

bool Date::operator==(const Date &rhs) const
{
    return this->day_ == rhs.day_ && this->month_ == rhs.month_ && this->year_ == rhs.year_;
}

bool Date::operator!=(const Date &rhs) const
{
    return !(*this == rhs);
}

bool Date::operator>(const Date &rhs) const
{
    if (this->year_ > rhs.year_)
    {
        return true;
    }
    else if (this->year_ == rhs.year_)
    {
        if (this->month_ > rhs.month_)
        {
            return true;
        }
        else if (this->month_ == rhs.month_)
        {
            if (this->day_ > rhs.day_)
            {
                return true;
            }
        }
    }

    return false;
}

bool Date::operator<(const Date &rhs) const
{
    return !(*this > rhs);
}

bool Date::operator>=(const Date &rhs) const
{
    return (*this > rhs) || (*this == rhs);
}

bool Date::operator<=(const Date &rhs) const
{
    return (*this < rhs) || (*this == rhs);
}

ProtoDate Date::ToProtoDate() const {
    ProtoDate proto_date;
    proto_date.set_day(day_);
    proto_date.set_month(month_);
    proto_date.set_year(year_);
    return proto_date;
}

void Date::FromProtoDate(const ProtoDate &date)
{
    day_ = date.day();
    month_ = date.month();
    year_ = date.year();
}
}