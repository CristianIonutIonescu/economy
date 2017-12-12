#ifndef DATE_HPP
#define DATE_HPP

#include <string>
#include <vector>

namespace economy
{
class Date
{
public:
  Date() : day_(1), month_(1), year_(1970) {}

  Date(int day, int month, int year) : day_(day), month_(month), year_(year) {}

  Date(const std::string &date, const std::string &format = "DD-MM-YYYY") { FromString(date, format); }

  Date(const Date &rhs) : day_(rhs.day_), month_(rhs.month_), year_(rhs.year_) {}

  Date(const Date &&rhs) : day_(std::move(rhs.day_)), month_(std::move(rhs.month_)), year_(std::move(rhs.year_)) {}

  Date operator=(const Date &rhs)
  {
    this->day_ = rhs.day_;
    this->month_ = rhs.month_;
    this->year_ = rhs.year_;
  }

  Date operator=(const Date &&rhs)
  {
    this->day_ = std::move(rhs.day_);
    this->month_ = std::move(rhs.month_);
    this->year_ = std::move(rhs.year_);
  }

  bool operator==(const Date &rhs) const;
  bool operator!=(const Date &rhs) const;
  bool operator>(const Date &rhs) const;
  bool operator<(const Date &rhs) const;
  bool operator>=(const Date &rhs) const;
  bool operator<=(const Date &rhs) const;

  std::string ToString(const std::string &format = "DD-MM-YYYY") const;
  void FromString(const std::string &date, const std::string &format = "DD-MM-YYYY");

private:
  const std::vector<std::string> date_formats{
      "DD-MM-YYY", "DD/MM/YYYY", "MM-DD-YYYY", "MM/DD/YYYY", "YYYY-MM-DD", "YYYY/MM/DD",
      ///"DD-MMM-YYY", "DD/MMM/YYYY", "MMM-DD-YYYY", "MMM/DD/YYYY", "YYYY-MMM-DD", "YYYY/MMM/DD"
  };
  int day_;
  int month_;
  int year_;
};
}
#endif