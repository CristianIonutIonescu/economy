#ifndef DATAPRESENTER_HPP
#define DATAPRESENTER_HPP

#include "chart.hpp"
#include "dataretriever.hpp"

#include <QtWidgets/QtWidgets>

namespace economy
{
namespace client
{

class DataPresenter : public QWidget
{
  Q_OBJECT
public:
  DataPresenter();
private slots:
  void RetrieveData();

  void RetrieveCurrency();

private:
  void CreateChartGroupBox();

  void CreateOptionsGroupBox();

  void CreateDateGroupBox();

  void CreateCurrencyGroupBox();

  std::unique_ptr<QGroupBox> chartGroupBox_;
  std::unique_ptr<QGridLayout> chartLayout_;
  std::unique_ptr<QtCharts::QChartView> chartView_;
  std::unique_ptr<EconomyChartModel> model_;
  DataRetriever retriever_;

  std::unique_ptr<QGroupBox> optionsGroupBox_;
  std::unique_ptr<QGridLayout> optionsLayout_;

  std::unique_ptr<QGroupBox> dateGroupBox_;
  std::unique_ptr<QGridLayout> dateLayout_;
  std::unique_ptr<QLabel> beginDateLabel_;
  std::unique_ptr<QDateEdit> beginDateEdit_;
  std::unique_ptr<QLabel> endDateLabel_;
  std::unique_ptr<QDateEdit> endDateEdit_;
  std::unique_ptr<QPushButton> getDataButton_;

  std::unique_ptr<QGroupBox> currencyGroupBox_;
  std::unique_ptr<QGridLayout> currencyLayout_;
  std::unique_ptr<QLabel> newCurrencyLabel_;
  std::unique_ptr<QComboBox> newCurrencyCombo_;
  std::unique_ptr<QStringListModel> newCurrencyModel_;
  std::unique_ptr<QPushButton> getNewCurrency_;
  std::unique_ptr<QLabel> currentCurrencyLabel_;
  std::unique_ptr<QLabel> currentCurrency_;

  std::unique_ptr<QGroupBox> infoGroupBox_;
  std::unique_ptr<QGridLayout> infoLayout_;
  std::unique_ptr<QLabel> amountLabel_;
  std::unique_ptr<QLabel> euroToRonLabel_;
  std::unique_ptr<QLabel> usdToRonLabel_;

  std::unique_ptr<QGridLayout> layout_;
};
}
}
#endif