#ifndef DATAPRESENTER_HPP
#define DATAPRESENTER_HPP

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

namespace economy
{
namespace client
{

class DataRetriever;

class Presenter
{
  public:
    explicit Presenter(DataRetriever *retriever) : retriever_(retriever) {}
    QChartView * Run();

  private:
    DataRetriever *retriever_;
};
}
}
#endif