#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_market/s4MarketDataSource.h"

#include "ui_s4MarketDataSource.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>
#include <QSortFilterProxyModel>
#include <QTableWidget>
#include <QPushButton>

using namespace std;

namespace S4 {
namespace QT {


marketDataSource::marketDataSource(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::marketDataSource)
{   
	ui->setupUi(this);

	connect(ui->StartButton, &QPushButton::pressed, this, &marketDataSource::onStart);
	connect(ui->AddButton, &QPushButton::pressed, this, &marketDataSource::onAdd);
    ui->AddButton->setDisabled(true);
    ui->ImportButton->setDisabled(true);
    ui->ExportButton->setDisabled(true);

	setMaximumWidth(260);
	setMaximumHeight(260);

	_started = false;
}


void marketDataSource::onStart()
{
	if (_started) {
		ui->StartButton->setText(QStringLiteral("开始"));
		emit signal_stop();
		_started = false;
	}
	else {
		ui->StartButton->setText(QStringLiteral("停止"));
		emit signal_start();
		_started = true;
	}
}

void marketDataSource::onAdd()
{
    QString Name("Costem");
    Name += QString::number(ui->tabWidget->count());

    marketDataSourceCfg* sourceCfg = new marketDataSourceCfg(Name, this);
    int i = ui->tabWidget->addTab(sourceCfg, Name);
    ui->tabWidget->setCurrentIndex(i);

	connect(this, &marketDataSource::signal_start, sourceCfg, &marketDataSourceCfg::slot_disableEdit);
	connect(this, &marketDataSource::signal_stop, sourceCfg, &marketDataSourceCfg::slot_enableEdit);
	connect(sourceCfg, &marketDataSourceCfg::signal_fixed, this, &marketDataSource::onSourceNameChange);
}

void marketDataSource::onSourceNameChange(QWidget* tab)
{
	int i = ui->tabWidget->indexOf(tab);
	ui->tabWidget->setTabText(i, ((marketDataSourceCfg*)tab)->label());
}

QList<marketDataSourceCfg::cfg_t> marketDataSource::getCfgs(void)
{
	QList<marketDataSourceCfg::cfg_t> ret;
	for (int i=0; i<ui->tabWidget->count();++i)
	{
		marketDataSourceCfg* tab = (marketDataSourceCfg*)ui->tabWidget->widget(i);
		ret.append(tab->getCfg());
	}
	return ret;
}


marketDataSource::~marketDataSource()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
