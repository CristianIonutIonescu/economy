#include "datapresenter.hpp"

#include "dataretriever.hpp"

QT_CHARTS_USE_NAMESPACE

namespace economy
{
namespace client
{


QChartView* Presenter::Run()
{
    QLineSeries *series = new QLineSeries();

    Date beg("12-03-2017");
    Date end("12-05-2017");

    try {
        auto data = retriever_->GetData(beg,end);

        for(const auto it: data) {
            QDateTime momentInTime;
            momentInTime.setSecsSinceEpoch(it.first.SecsSinceEpoch());
            series->append(momentInTime.toMSecsSinceEpoch(),it.second);
        }
    }catch(const std::exception &ex) {
        std::cerr<<ex.what()<<std::endl;
    }

    //![3]
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    //![3]

    //![4]
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Sunspots count");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    //![4]

    //![5]
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}
}
}