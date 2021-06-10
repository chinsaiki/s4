#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapMarketDataSource.h"

#include "ui_s4SnapMarketDataSource.h"

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


snapMarketDataSource::snapMarketDataSource(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::snapMarketDataSource)
{   
	ui->setupUi(this);

	connect(ui->StartButton, &QPushButton::pressed, this, &snapMarketDataSource::onStart);
	connect(ui->AddButton, &QPushButton::pressed, this, &snapMarketDataSource::onAdd);
    ui->AddButton->setDisabled(true);
    ui->ImportButton->setDisabled(true);
    ui->ExportButton->setDisabled(true);

	_started = false;
}


void snapMarketDataSource::onStart()
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

void snapMarketDataSource::onAdd()
{
    QString Name("Costem");
    Name += QString::number(ui->tabWidget->count());

    snapMarketDataSourceCfg* sourceCfg = new snapMarketDataSourceCfg(Name, this);
    int i = ui->tabWidget->addTab(sourceCfg, Name);
    ui->tabWidget->setCurrentIndex(i);

	connect(this, &snapMarketDataSource::signal_start, sourceCfg, &snapMarketDataSourceCfg::slot_disableEdit);
	connect(this, &snapMarketDataSource::signal_stop, sourceCfg, &snapMarketDataSourceCfg::slot_enableEdit);
	connect(sourceCfg, &snapMarketDataSourceCfg::signal_fixed, this, &snapMarketDataSource::onSourceNameChange);
}

void snapMarketDataSource::onSourceNameChange(QWidget* tab)
{
	int i = ui->tabWidget->indexOf(tab);
	ui->tabWidget->setTabText(i, ((snapMarketDataSourceCfg*)tab)->label());
}

QList<snapMarketDataSourceCfg::cfg_t> snapMarketDataSource::getCfgs(void)
{
	QList<snapMarketDataSourceCfg::cfg_t> ret;
	for (int i=0; i<ui->tabWidget->count();++i)
	{
		snapMarketDataSourceCfg* tab = (snapMarketDataSourceCfg*)ui->tabWidget->widget(i);
		ret.append(tab->getCfg());
	}
	return ret;
}


snapMarketDataSource::~snapMarketDataSource()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
