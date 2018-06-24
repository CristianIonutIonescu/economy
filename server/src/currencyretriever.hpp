#ifndef CURRENCYRETRIEVER_HPP
#define CURRENCYRETRIEVER_HPP

#include "../../common/date.hpp"

#include <atomic>
#include <map>
#include <mutex>
#include <string>

namespace economy
{
namespace server
{

class CurrencyRetriever
{
  private:
    struct CurrencyEntry
    {
        float euro;
        float usd;
    };

  public:
    explicit CurrencyRetriever(std::mutex *lock,
                               const std::string &db_path,
                               const std::string &script_path);

    CurrencyRetriever(const CurrencyRetriever &) = delete;
    CurrencyRetriever operator=(const CurrencyRetriever &) = delete;

    ~CurrencyRetriever() = default;

    bool RequestCurrency();

    Currency GetCurrency(CurrencyType type) const;

    inline void Shutdown() noexcept { stop_ = true; }

    inline bool Closing() const noexcept { return stop_; }

  private:
    void DigestData(const std::string &content);
    void LoadData();

    std::string script_path_;
    std::string db_path_;
    std::map<economy::Date, CurrencyEntry> data_;
    std::mutex *data_lock_;
    std::atomic<bool> stop_;
};
}
}
#endif