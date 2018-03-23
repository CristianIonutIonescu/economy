#include "currencyretriever.hpp"

#include "../../common/utils.hpp"
#include <process.hpp>

#include <fstream>

namespace economy
{
namespace server
{

CurrencyRetriever::CurrencyRetriever(std::mutex *lock,
                                     const std::string &db_path,
                                     const std::string &script_path)
    : data_lock_(lock),
      script_path_("python " + script_path),
      db_path_(db_path)
{
}

// TODO: handle io exceptions
void CurrencyRetriever::LoadData()
{
    std::ifstream file(db_path_);

    std::string line;
    try
    {
        while (std::getline(file, line))
        {
            if (line.empty())
            {
                break;
            }

            DigestData(line);
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Could not read currency archive: " << ex.what()
                  << std::endl;
    }

    file.close();
}

//TODO: handle io exceptions
void CurrencyRetriever::SaveData()
{
    std::ofstream file(db_path_);

    for (const auto &it : data_)
    {
        file << it.first.ToString("YYYY-MM-DD") << "," << it.second.euro << ","
             << it.second.usd << std::endl;
    }

    file.close();
}

CurrencyRetriever::~CurrencyRetriever()
{
    SaveData();
}

bool CurrencyRetriever::RequestCurrency()
{
    std::string content;
    auto stdout_pipe = [&content](const char *bytes, size_t n) {
        content = std::string(bytes, n);
    };
    TinyProcessLib::Process retriever_process(script_path_, "", stdout_pipe);

    DigestData(content);

    SaveData();

    return true;
}

Currency CurrencyRetriever::GetCurrency(CurrencyType type) const
{
    auto curr = data_.rbegin();
    Currency currency;
    currency.set_type(type);

    switch (type)
    {
    case CurrencyType::EUR:
        currency.set_conversion_rate(curr->second.euro);
        break;
    case CurrencyType::USD:
        currency.set_conversion_rate(curr->second.usd);
        break;
    }

    return currency;
}

//TODO: handle exceptions.
void CurrencyRetriever::DigestData(const std::string &content)
{
    auto info = ParseCSVLine(content, ',');
    CurrencyEntry rates;
    rates.euro = std::stof(info[1]);
    rates.usd = std::stof(info[2]);
    data_.emplace(std::make_pair(Date(info[0], "YYYY-MM-DD"), rates));
}
}
}