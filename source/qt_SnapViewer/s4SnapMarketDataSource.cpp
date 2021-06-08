#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapMarketDataSource.h"
#include "qt_SnapViewer/s4SnapMarketDataSourceCfg.h"

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

    connect(ui->AddButton, &QPushButton::pressed, this, &snapMarketDataSource::onAdd);

}

void snapMarketDataSource::onAdd()
{
    snapMarketDataSourceCfg* sourceCfg = new snapMarketDataSourceCfg(this);
    QString Name("Costem");
    Name += QString::number(ui->tabWidget->count());
    int i = ui->tabWidget->addTab(sourceCfg, Name);
    ui->tabWidget->setCurrentIndex(i);
}


snapMarketDataSource::~snapMarketDataSource()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
