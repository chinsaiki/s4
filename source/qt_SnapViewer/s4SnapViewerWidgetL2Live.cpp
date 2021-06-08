#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapViewerWidgetL2Live.h"
#include "qt_common/s4qt_itemDelegateNumberOnly.h"
#include "network/L2_udp_recver_th_native.h"

#include "common/s4logger.h"

#include <QSplitter>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaType>
#include <QStyleFactory>
#include <QSortFilterProxyModel>
#include <QTableWidget>
#include <QHeaderView>
#include <QGridLayout>
#include <QDebug>

using namespace std;

namespace S4 {
namespace QT {

//CREATE_LOCAL_LOGGER("qt_SnapViewer")
#define AIM_SECURITY_TREE_NAME QStringLiteral("双击添加代码")
Q_DECLARE_METATYPE(NW::L2Stats_t);
Q_DECLARE_METATYPE(std::string);

bool transCode(const QString& raw_code, QString& mktCode)
{
    QString code = raw_code.toLower();
    if (code.size() < 8){
        if (code.left(2) == "sz"){
            code.insert(2, QString(8-code.size(), '0'));
        }else if(code.left(2) == "sh"){
            code.insert(2, '6');
            if (code.size() < 8){
                code.insert(3, QString(8-code.size(), '0'));
            }
        }
    }
    std::string mktCodeStr;
    // mktCodeI_t mktCodeI;
    try{
        // mktCodeI = 
        mktCodeStr_to_mktCodeInt(code.toStdString());
        mktCode = code;
        return true;
    }catch(Exception& e){
        qDebug() << e.what();
    }
    try{
        mktCodeStr = pureCodeStr_to_mktCodeStr(code.toStdString());
        mktCode = QString::fromStdString(mktCodeStr);
        // mktCodeI = 
        mktCodeStr_to_mktCodeInt(mktCodeStr);
        return true;
    }catch(Exception& e){
		qDebug() << e.what();
	}
    return false;
}

s4SnapViewerWidgetL2Live::s4SnapViewerWidgetL2Live(QWidget *parent) :
    s4SnapViewerWidget(parent)
{
	_treeView = new unicTreeView(this);
	_treeView->setStyle(QStyleFactory::create("windows"));
	_treeView->setSortingEnabled(true);
	_treeView->setMaximumWidth(150);
	_treeView->setItemDelegate( new itemDelegateNumberOnly);
    connect(_treeView->itemDelegate(), &QAbstractItemDelegate::closeEditor, (unicTreeView*)_treeView, &unicTreeView::onItemChanged);

	_tabWidget = new QTabWidget(this);
    _tabWidget->setTabsClosable(true);

    itemFormatDelegate* delegate = new itemFormatDelegate(this);
    _stats_tv = new QTableView(this);
    _stats_tv->setItemDelegate(delegate);
    _stats_model = new snapTableModel_L2Stats(_stats_tv);
    _stats_tv->setModel(_stats_model);
    _stats_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _stats_tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // _stats_tv->verticalHeader()->setMaximumHeight(24);
    // _stats_tv->verticalHeader()->setMinimumHeight(5);
    _stats_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
	_stats_tv->setMaximumWidth(300);


	QSplitter* splitter = new QSplitter(this);
	splitter->addWidget(_treeView);
	splitter->addWidget(_stats_tv);
	splitter->addWidget(_tabWidget);
    QList<int> list;
    list<<50<<50<<200;//width 为 50 100 200
    splitter->setSizes(list);

    //网格分割
	QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(splitter);
	setLayout(pLayout);

	connect(_tabWidget, &QTabWidget::tabCloseRequested, this, &s4SnapViewerWidget::closeSnapTab);
	connect(_tabWidget, &QTabWidget::tabCloseRequested, this, &s4SnapViewerWidgetL2Live::closeSnapTab);

	// connect((unicTreeView*)_treeView, &unicTreeView::, this, &s4SnapViewerWidgetL2Live::dbTree_doubleClicked);
	connect((unicTreeView*)_treeView, &unicTreeView::signal_newItem, this, &s4SnapViewerWidgetL2Live::openInstrumentTab);
	connect((unicTreeView*)_treeView, &unicTreeView::signal_delItem, this, &s4SnapViewerWidgetL2Live::closeInstrumentTab);
	connect((unicTreeView*)_treeView, &unicTreeView::signal_selectItem, this, &s4SnapViewerWidgetL2Live::setCurrentInstrument);
	connect(this, &s4SnapViewerWidgetL2Live::signal_closeSnapTab, (unicTreeView*)_treeView, &unicTreeView::onDelItem);

	newTree(AIM_SECURITY_TREE_NAME, {});
	_aim_security_root = _tree_model->findItems(AIM_SECURITY_TREE_NAME).first();
	_aim_security_root->setFlags(_aim_security_root->flags() & (~Qt::ItemIsEditable));
    ((unicTreeView*)_treeView)->onSetCurrentRoot(_aim_security_root);
	((unicTreeView*)_treeView)->onSetTextFormater(&transCode);

    startDataLive();
}
void s4SnapViewerWidgetL2Live::startDataLive()
{
	if (!_pL2DataQ) {
		_pL2DataQ = std::make_shared<NW::L2DataQ_t>(1024, 2048, true);
	}
	if (!_pL2CmdQ) {
		_pL2CmdQ = std::make_shared<NW::L2CmdQ_t>(64);
	}
	_snapMarketDataLive = new s4SnapMarketDataLive(_pL2DataQ, _pL2CmdQ);

	qRegisterMetaType<struct NW::L2Stats_t>();
	connect(_snapMarketDataLive, &s4SnapMarketDataLive::signal_L2Stats, _stats_model, &snapTableModel_L2Stats::refresh);
	_snapMarketDataLive->start();
}


void s4SnapViewerWidgetL2Live::onStartL2LiveReceiver()
{
	if (!_udp_recver_th) {
		_udp_recver_th = std::make_shared<NW::L2_udp_recver_th_native>(_pL2DataQ, _pL2CmdQ);
	}
	_udp_recver_th->start("0.0.0.0", 8888);
	for (int i = 0; i < _tabWidget->count(); ++i) {
		_snapMarketDataLive->addLive(mktCodeStr_to_mktCodeInt(_tabWidget->tabText(i).toStdString()));
	}
}

void s4SnapViewerWidgetL2Live::onStopL2LiveReceiver()
{
	if (_udp_recver_th) {
		_udp_recver_th->stop();
	}
}

// void s4SnapViewerWidgetL2Live::dbTree_doubleClicked(const QModelIndex& index={}) {
// 	//if (!index.parent().isValid())
// 	//	return;
// 	//if (!index.parent().parent().isValid()) return;

//     QStandardItem * item = new QStandardItem;
// 	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
// 	_aim_security_root->appendRow(item);
    
//     _treeView->edit(item->index());
// 	item->setFlags(item->flags() ^ Qt::ItemIsEditable);
// }


void s4SnapViewerWidgetL2Live::openInstrumentTab(const QString& code)
{
    QString mktCode;
    if (_instrument_info_cargo.count(code) == 0){
        snapInstrument* pInstrument = new snapInstrument(10, this);
	    qRegisterMetaType<std::string>();
		connect(_snapMarketDataLive, &s4SnapMarketDataLive::signal_L2Data_instrument_snap, pInstrument, &snapInstrument::onL2Data_instrument_snap);
		connect(_snapMarketDataLive, &s4SnapMarketDataLive::signal_L2Data_index_snap, pInstrument, &snapInstrument::onL2Data_index_snap);
		connect(_snapMarketDataLive, &s4SnapMarketDataLive::signal_L2Data_order, pInstrument, &snapInstrument::onL2Data_order);
		connect(_snapMarketDataLive, &s4SnapMarketDataLive::signal_L2Data_exec, pInstrument, &snapInstrument::onL2Data_exec);
        openSnapTab(code, pInstrument);
        snap_info_t info;
        info.code = mktCodeStr_to_mktCodeInt(code.toStdString());
        _instrument_info_cargo[code] = info;

        _snapMarketDataLive->addLive(info.code);
	}
}

void s4SnapViewerWidgetL2Live::setCurrentInstrument(const QString& code)
{
	for (int i = 0; i < _tabWidget->count(); ++i) {
		if (_tabWidget->tabText(i) == code) {
			_tabWidget->setCurrentIndex(i);
		}
	}
}

void s4SnapViewerWidgetL2Live::closeInstrumentTab(const QString& code)
{
    for (int i = 0; i < _tabWidget->count(); ++i) {
		if (_tabWidget->tabText(i) == code) {
			_tabWidget->removeTab(i);
        }
    }

	auto it = _instrument_info_cargo.find(code);
	if (it != _instrument_info_cargo.end()) {
		_snapMarketDataLive->delLive(it->second.code);
		_instrument_info_cargo.erase(code);
	}
}

void s4SnapViewerWidgetL2Live::closeSnapTab(int index)
{
	const QString code = _tabWidget->tabText(index);
	auto it = _instrument_info_cargo.find(code);
	if (it != _instrument_info_cargo.end()) {
		_snapMarketDataLive->delLive(it->second.code);
		_instrument_info_cargo.erase(code);
	}

    s4SnapViewerWidget::closeSnapTab(index);
    emit signal_closeSnapTab(code);
}


s4SnapViewerWidgetL2Live::~s4SnapViewerWidgetL2Live()
{
	_snapMarketDataLive->stop();
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
