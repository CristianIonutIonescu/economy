#ifndef CHART_HPP
#define CHART_HPP

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

#include <memory>
#include <vector>
#include "../common/date.hpp"
#include "../common/datatypes.grpc.pb.h"

#include <iostream>

namespace economy
{
namespace client
{

class EconomyChartModel : public QtCharts::QChart
{
public:
  EconomyChartModel();

  EconomyChartModel(const EconomyChartModel &other) = delete;
  EconomyChartModel operator=(const EconomyChartModel &other) = delete;

  void AddData(const std::vector<std::pair<Date, float>> &data);
  void ChangeCurrency(const Currency &new_currency);
  //inline QDateTime MaxXValue() const { return xAxis_->max(); }
  //inline QDateTime MinXValue() const { return xAxis_->min(); }
  //inline qreal MaxYValue() const { return yAxis_->max(); }
  //inline qreal MinYValue() const { return yAxis_->min(); }

private:
  void InitXAxis();
  void InitYAxis();

  std::unique_ptr<QtCharts::QDateTimeAxis> xAxis_;
  std::unique_ptr<QtCharts::QValueAxis> yAxis_;
  std::unique_ptr<QtCharts::QLineSeries> series_;
  std::vector<std::pair<Date, float>> data_;
  Currency currentCurrency_;
};
}
}
#endif