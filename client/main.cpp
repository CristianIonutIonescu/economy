#include "dataretriever.hpp"
#include "datapresenter.hpp"

//#define DEBUG_MOD 1

using namespace economy;
using namespace economy::client;

int main(int argc ,char **argv) {
    QApplication a(argc, argv);
    DataPresenter mainwindow;
    mainwindow.show();

    return a.exec();

}