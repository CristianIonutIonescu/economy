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
      db_path_(db_path),
      stop_(false)
{
    LoadData();
}

void CurrencyRetriever::LoadData()
{
    const auto temp_data = data_;

    std::ifstream file(db_path_);

    if (!file.good())
    {
        return;
    }

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

            DigestData(line);
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Could not read currency archive: " << ex.what()
                  << std::endl << "Using old data instead" << std::endl;
        data_ = temp_data;
    }

    file.close();
}

bool CurrencyRetriever::RequestCurrency()
{
    bool success = true;
    auto stderr_pipe = [&success](const char *bytes, size_t n) {
        std::string error_msg = std::string(bytes, n);
        if (error_msg != "")
        {
            success = false;
            std::cerr << error_msg << std::endl;
        }
    };
    TinyProcessLib::Process retriever_process(script_path_, "", nullptr, stderr_pipe);

    return success;
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

void CurrencyRetriever::DigestData(const std::string &content)
{
    auto info = ParseCSVLine(content, ',');
    if (data_.rbegin()->first.ToString("YYYY-MM-DD") == info[0])
    {
        return;
    }

    CurrencyEntry rates;
    try
    {
        rates.euro = std::stof(info[1]);
        rates.usd = std::stof(info[2]);
        data_.emplace(std::make_pair(Date(info[0], "YYYY-MM-DD"), rates));
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error digesting line '" << content << "': " << ex.what()
                  << std::endl;
    }
}
}
}