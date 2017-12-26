#include <iostream>
#include "dataretriever.hpp"
#include "datapresenter.hpp"

using namespace economy;
using namespace economy::client;
QT_CHARTS_USE_NAMESPACE

int main(int argc ,char **argv) {
    QApplication a(argc, argv);
    DataRetriever test("localhost:8080");
    Presenter viewer(&test);
    QChartView *chartView = viewer.Run();
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(820, 600);
    window.show();

    return a.exec();

}