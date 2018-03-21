#ifndef CURRENCYRETRIEVER_HPP
#define CURRENCYRETRIEVER_HPP

#include "../../common/date.hpp"

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
     
    explicit CurrencyRetriever(std::mutex *lock, const std::string &db_path, const std::string &script_path);

    CurrencyRetriever(const CurrencyRetriever &) = delete;
    CurrencyRetriever operator=(const CurrencyRetriever &) = delete;

    ~CurrencyRetriever() noexcept;

    bool RequestCurrency();

    Currency GetCurrency(CurrencyType type) const;

    inline void Shutdown() noexcept { closing_ = true; }

    inline bool Closing() const noexcept { return closing_; }
  private:
    static constexpr auto s_db_filename = "currency_db.dat";
    static constexpr auto s_script_filename = "currencyretriever.py";

    void DigestData(const std::string &content);
    void LoadData();
    void SaveData();

    std::string script_path_;
    std::string db_path_;
    std::map<economy::Date, CurrencyEntry> data_;
    std::mutex* data_lock_;
    bool closing_;
};
}
}
#endif