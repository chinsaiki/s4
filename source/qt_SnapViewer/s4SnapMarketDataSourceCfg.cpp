#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapMarketDataSource.h"
#include "qt_SnapViewer/s4SnapMarketDataSourceCfg.h"

#include "ui_s4SnapMarketDataSourceCfg.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>
#include <QSortFilterProxyModel>
#include <QTableWidget>

using namespace std;

namespace S4 {
namespace QT {


snapMarketDataSourceCfg::snapMarketDataSourceCfg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::snapMarketDataSourceCfg)
{   
	ui->setupUi(this);


}




snapMarketDataSourceCfg::~snapMarketDataSourceCfg()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
