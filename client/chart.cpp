#include "chart.hpp"
#include <iostream>

namespace economy
{
namespace client
{
EconomyChartModel::EconomyChartModel()
{
    this->setTheme(QtCharts::QChart::ChartTheme::ChartThemeDark);

    currentCurrency_.set_type(CurrencyType::RON);
    currentCurrency_.set_conversion_rate(1.0);
    legend()->hide();

}

void EconomyChartModel::AddData(const std::vector<std::pair<Date, float>> &data)
{
    data_ = data;
    if (series_)
    {
        this->removeSeries(series_.get());
        series_.reset();
    }

    float value = 0;
    series_ = std::make_unique<QtCharts::QLineSeries>();
    for (const auto it : data_)
    {
        QDateTime momentInTime;
        momentInTime.setSecsSinceEpoch(it.first.ToEpochTime());
        double conversion_rate = currentCurrency_.conversion_rate();
        float converted_value = it.second / conversion_rate;
        value +=converted_value;
        series_->append(momentInTime.toMSecsSinceEpoch(), value);
#ifdef DEBUG_MOD
        std::cout<<momentInTime.toString().toStdString()<<":" << it.second << std::endl;
#endif
    }

#ifdef DEBUG_MOD
    std::cout<<"Value:"<< value <<std::endl;
#endif

    this->addSeries(series_.get());

    InitXAxis();
    InitYAxis();
}

void EconomyChartModel::ChangeCurrency(const Currency &new_currency)
{
    currentCurrency_ = new_currency;
    AddData(data_);
}

void EconomyChartModel::InitXAxis()
{
    if (xAxis_)
    {
        removeAxis(xAxis_.get());
        xAxis_.reset();
    }
    xAxis_ = std::make_unique<QtCharts::QDateTimeAxis>();
    xAxis_->setTitleText("Period");
    xAxis_->setFormat("dd-MM-yy");
    this->addAxis(xAxis_.get(), Qt::AlignBottom);
    series_->attachAxis(xAxis_.get());
}
void EconomyChartModel::InitYAxis()
{
    if (yAxis_)
    {
        removeAxis(yAxis_.get());
        yAxis_.reset();
    }
    yAxis_ = std::make_unique<QtCharts::QValueAxis>();
    yAxis_->setTitleText("Amount");
    this->addAxis(yAxis_.get(), Qt::AlignLeft);
    series_->attachAxis(yAxis_.get());
}
}
}