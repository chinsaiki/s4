﻿#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4189) 
#endif

#include "qt_SnapViewer/s4SnapViewerWidgetL2Live.h"
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

using namespace std;

namespace S4 {
namespace QT {

//CREATE_LOCAL_LOGGER("qt_SnapViewer")
#define AIM_SECURITY_TREE_NAME QStringLiteral("双击添加代码")

s4SnapViewerWidgetL2Live::s4SnapViewerWidgetL2Live(QWidget *parent) :
    s4SnapViewerWidget(parent)
{
	_treeView = new QTreeView(this);
	_treeView->setStyle(QStyleFactory::create("windows"));
	_treeView->setSortingEnabled(true);
	_treeView->setMaximumWidth(150);

	_tabWidget = new QTabWidget(this);

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

	connect(_treeView, &QTreeView::doubleClicked, this, &s4SnapViewerWidgetL2Live::dbTree_doubleClicked);

	newTree(AIM_SECURITY_TREE_NAME, {});
	_aim_security_root = _tree_model->findItems(AIM_SECURITY_TREE_NAME).first();
    
}

Q_DECLARE_METATYPE(NW::L2Stats_t);

void s4SnapViewerWidgetL2Live::onStartL2LiveReceiver()
{
	if (!_pL2DataQ) {
		_pL2DataQ = std::make_shared<NW::L2DataQ_t>(1024, 2048, true);
	}
	if (!_pL2CmdQ) {
		_pL2CmdQ = std::make_shared<NW::L2CmdQ_t>(64);
	}

	if (!_udp_recver_th) {
		_udp_recver_th = std::make_shared<NW::L2_udp_recver_th_native>(_pL2DataQ, _pL2CmdQ);
	}
    _snapMarketDataLive = new s4SnapMarketDataLive(_pL2DataQ, _pL2CmdQ);

    qRegisterMetaType<struct NW::L2Stats_t>();
    connect(_snapMarketDataLive, &s4SnapMarketDataLive::onL2Stats, 
            _stats_model, &snapTableModel_L2Stats::refresh);
    _snapMarketDataLive->start();
    _udp_recver_th->start("0.0.0.0", 8888);
}

void s4SnapViewerWidgetL2Live::onStopL2LiveReceiver()
{
	if (_udp_recver_th) {
		_udp_recver_th->stop();
	}
    _snapMarketDataLive->stop();
}

void s4SnapViewerWidgetL2Live::dbTree_doubleClicked(const QModelIndex& index) {
	//if (!index.parent().isValid())
	//	return;
	//if (!index.parent().parent().isValid()) return;

    QStandardItem * item = new QStandardItem;
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsEditable);
	_aim_security_root->appendRow(item);
}

void s4SnapViewerWidgetL2Live::openInstrumentTab(mktCodeI_t)
{
}


void s4SnapViewerWidgetL2Live::closeSnapTab(int index)
{
	const QString tabName = _tabWidget->tabText(index);
	_tabWidget->removeTab(index);

	auto it = _instrument_info_cargo.find(tabName);
	if (it != _instrument_info_cargo.end()) {
		_instrument_info_cargo.erase(tabName);
	}
}

s4SnapViewerWidgetL2Live::~s4SnapViewerWidgetL2Live()
{
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
