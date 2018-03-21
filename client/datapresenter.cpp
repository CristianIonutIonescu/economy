#include "datapresenter.hpp"

#include "../common/utils.hpp"

namespace economy
{
namespace client
{

DataPresenter::DataPresenter() : retriever_("localhost:8080")
{

    CreateChartGroupBox();
    CreateOptionsGroupBox();

    layout_ = std::make_unique<QGridLayout>();
    layout_->addWidget(chartGroupBox_.get(), 0, 0);
    layout_->addWidget(optionsGroupBox_.get(), 1, 0);

    setLayout(layout_.get());
    RetrieveData();
}

void DataPresenter::CreateChartGroupBox()
{
    chartGroupBox_ = std::make_unique<QGroupBox>();
    model_ = std::make_unique<EconomyChartModel>();

    chartView_ = std::make_unique<QtCharts::QChartView>(model_.get());
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    chartView_->setMinimumSize(screenGeometry.width()/2, screenGeometry.height()/2);

    chartLayout_ = std::make_unique<QGridLayout>();
    chartLayout_->addWidget(chartView_.get(), 0, 0);
    chartGroupBox_->setLayout(chartLayout_.get());
}

void DataPresenter::CreateOptionsGroupBox()
{
    CreateDateGroupBox();
    CreateCurrencyGroupBox();
    optionsGroupBox_ = std::make_unique<QGroupBox>();
    optionsLayout_ = std::make_unique<QGridLayout>();

    optionsLayout_->addWidget(dateGroupBox_.get(), 0, 0);
    optionsLayout_->addWidget(currencyGroupBox_.get(), 0, 1);

    optionsGroupBox_->setLayout(optionsLayout_.get());
}

void DataPresenter::CreateDateGroupBox()
{
    dateGroupBox_ = std::make_unique<QGroupBox>(tr("Dates"));
    getDataButton_ = std::make_unique<QPushButton>(tr("Retrieve Data"), this);
    connect(getDataButton_.get(), &QPushButton::clicked, this, &DataPresenter::RetrieveData);

    beginDateEdit_ = std::make_unique<QDateEdit>();
    beginDateEdit_->setDisplayFormat("d MMM yyyy");
    beginDateEdit_->setDate(QDate::currentDate().addMonths(-1));

    beginDateLabel_ = std::make_unique<QLabel>(tr("Begin Date:"));
    beginDateLabel_->setBuddy(beginDateEdit_.get());

    endDateEdit_ = std::make_unique<QDateEdit>();
    endDateEdit_->setDisplayFormat("d MMM yyyy");
    endDateEdit_->setDate(QDate::currentDate());

    endDateLabel_ = std::make_unique<QLabel>(tr("End Date:"));
    endDateLabel_->setBuddy(endDateEdit_.get());

    dateLayout_ = std::make_unique<QGridLayout>();
    dateLayout_->addWidget(getDataButton_.get(), 0, 0, 2, 0);
    dateLayout_->addWidget(beginDateEdit_.get(), 2, 1);
    dateLayout_->addWidget(beginDateLabel_.get(), 2, 0);
    dateLayout_->addWidget(endDateEdit_.get(), 3, 1);
    dateLayout_->addWidget(endDateLabel_.get(), 3, 0);

    dateGroupBox_->setLayout(dateLayout_.get());
}

void DataPresenter::CreateCurrencyGroupBox()
{
    currencyGroupBox_ = std::make_unique<QGroupBox>(tr("Currency"));
    getNewCurrency_ = std::make_unique<QPushButton>(tr("Change Currency"));
    connect(getNewCurrency_.get(), &QPushButton::clicked, this, &DataPresenter::RetrieveCurrency);
    newCurrencyCombo_ = std::make_unique<QComboBox>();
    QStringList items;
    items << tr("RON") << tr("EUR") << tr("USD");
    newCurrencyModel_ = std::make_unique<QStringListModel>(items);
    newCurrencyCombo_->setModel(newCurrencyModel_.get());
    newCurrencyLabel_ = std::make_unique<QLabel>(tr("New Currency:"));
    newCurrencyLabel_->setBuddy(newCurrencyCombo_.get());

    currentCurrencyLabel_ = std::make_unique<QLabel>(tr("Current Currency:"));
    currentCurrency_ = std::make_unique<QLabel>(items[0]);
    currentCurrencyLabel_->setBuddy(currentCurrency_.get());

    currencyLayout_ = std::make_unique<QGridLayout>();
    currencyLayout_->addWidget(getNewCurrency_.get(), 0, 0, 2, 0);
    currencyLayout_->addWidget(currentCurrencyLabel_.get(), 2, 0);
    currencyLayout_->addWidget(currentCurrency_.get(), 2, 1);
    currencyLayout_->addWidget(newCurrencyLabel_.get(), 3, 0);
    currencyLayout_->addWidget(newCurrencyCombo_.get(), 3, 1);
    currencyGroupBox_->setLayout(currencyLayout_.get());
}

void DataPresenter::RetrieveData()
{
    auto qbegin = beginDateEdit_->dateTime();
    auto qend = endDateEdit_->dateTime();
    Date beg(qbegin.toString("dd-MM-yyyy").toStdString());
    Date end(qend.toString("dd-MM-yyyy").toStdString());
    try
    {
        auto data = retriever_.GetData(beg, end);
        model_->AddData(data);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

void DataPresenter::RetrieveCurrency()
{
    std::string new_currency_string = newCurrencyCombo_->currentText().toStdString();
    Currency new_currency;

    new_currency = retriever_.ChangeCurrency(CurrencyTypeFromString(new_currency_string));

    model_->ChangeCurrency(new_currency);
}
}
}