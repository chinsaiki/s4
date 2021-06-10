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
	//树表用于注册所关注的代码
	_treeView = new unicTreeView(this);
	_treeView->setStyle(QStyleFactory::create("windows"));
	_treeView->setSortingEnabled(true);
	_treeView->setMaximumWidth(150);
	_treeView->setItemDelegate( new itemDelegateNumberOnly(0, 999999, this));	//可输入范围 TODO: 允许sz/sh前缀，显示备选
	//编辑结束后触发代码合法性检查和自动补全
    connect(_treeView->itemDelegate(), &QAbstractItemDelegate::closeEditor, (unicTreeView*)_treeView, &unicTreeView::onItemChanged);

	//Tab用于显示行情
	_tabWidget = new QTabWidget(this);
    _tabWidget->setTabsClosable(true);

	//市场数据源 配置面板
	_marketDataSource = new snapMarketDataSource(this);
	_marketDataSource->setMaximumWidth(260);
	_marketDataSource->setMaximumHeight(220);
	_marketDataSource->onAdd();	//新增一个配置

	//表格用于显示市场数据状态
    _stats_tv = new QTableView(this);
    _stats_tv->setItemDelegate(new itemFormatDelegate(this));	//高亮变化值
    _stats_model = new snapTableModel_L2Stats(this);
    _stats_tv->setModel(_stats_model);
    _stats_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _stats_tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // _stats_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
	_stats_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);	//限制选择
	_stats_tv->setMaximumWidth(260);


	//数据源与统计列布局
	QSplitter* splitterStats = new QSplitter(Qt::Orientation::Vertical, this);	//竖排
	splitterStats->addWidget(_marketDataSource);
	splitterStats->addWidget(_stats_tv);
    QList<int> list;
    list<<150<<200;//width 为 1:2
	splitterStats->setSizes(list);

	//整体布局
	QSplitter* splitter = new QSplitter(this);	//横排
	splitter->addWidget(splitterStats);
	splitter->addWidget(_treeView);
	splitter->addWidget(_tabWidget);
    list<<50<<50<<200;//width 为 1:1:4
    splitter->setSizes(list);

    //布局放进网格，使填充满
	QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(splitter);
	setLayout(pLayout);

	//新建关注的代码列表，注册进树管理模块
	newTree(AIM_SECURITY_TREE_NAME, {});
	_aim_security_root = _tree_model->findItems(AIM_SECURITY_TREE_NAME).first();
	_aim_security_root->setFlags(_aim_security_root->flags() & (~Qt::ItemIsEditable));
    ((unicTreeView*)_treeView)->onSetCurrentRoot(_aim_security_root);
	((unicTreeView*)_treeView)->onSetTextFormater(&transCode);

	//代码行情tab关闭后，触发清理
	connect(_tabWidget, &QTabWidget::tabCloseRequested, this, &s4SnapViewerWidgetL2Live::closeSnapTab);
	connect(this, &s4SnapViewerWidgetL2Live::signal_closeSnapTab, (unicTreeView*)_treeView, &unicTreeView::onDelItem);	//传递tab的名称，即代码

	//关注的代码，新增、删除、选中时，触发tab动作
	connect((unicTreeView*)_treeView, &unicTreeView::signal_newItem, this, &s4SnapViewerWidgetL2Live::openInstrumentTab);
	connect((unicTreeView*)_treeView, &unicTreeView::signal_delItem, this, &s4SnapViewerWidgetL2Live::closeInstrumentTab);
	connect((unicTreeView*)_treeView, &unicTreeView::signal_selectItem, this, &s4SnapViewerWidgetL2Live::setCurrentInstrument);

	//市场数据源按下启动/停止时，触发建立/断开网络连接
	connect(_marketDataSource, &snapMarketDataSource::signal_start, this, &s4SnapViewerWidgetL2Live::slot_startMDSource);
	connect(_marketDataSource, &snapMarketDataSource::signal_stop, this, &s4SnapViewerWidgetL2Live::slot_stopMDSource);

	//开启市场数据代理线程
    startMDAgent();
}
void s4SnapViewerWidgetL2Live::startMDAgent()
{
	if (!_pL2DataQ) {
		_pL2DataQ = std::make_shared<NW::L2DataQ_t>(1024, 2048, true);
	}
	if (!_pL2CmdQ) {
		_pL2CmdQ = std::make_shared<NW::L2CmdQ_t>(64);
	}
	_snapMarketDataLive = new s4SnapMarketDataAgent(_pL2DataQ, _pL2CmdQ);

	qRegisterMetaType<struct NW::L2Stats_t>();
	connect(_snapMarketDataLive, &s4SnapMarketDataAgent::signal_L2Stats, _stats_model, &snapTableModel_L2Stats::refresh);
	_snapMarketDataLive->start();
}


void s4SnapViewerWidgetL2Live::slot_startMDSource()
{
	QList<snapMarketDataSourceCfg::cfg_t> sourceCfgs = _marketDataSource->getCfgs();

	if (!_udp_recver_th) {
		_udp_recver_th = std::make_shared<NW::L2_udp_recver_th_native>(_pL2DataQ, _pL2CmdQ);
	}
	_udp_recver_th->start(sourceCfgs[0].IP.c_str(), sourceCfgs[0].Port);
	for (int i = 0; i < _tabWidget->count(); ++i) {
		_snapMarketDataLive->addLive(mktCodeStr_to_mktCodeInt(_tabWidget->tabText(i).toStdString()));
	}
}

void s4SnapViewerWidgetL2Live::slot_stopMDSource()
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
    if (_instrument_info_cargo.count(code) == 0){
        snapInstrument* pInstrument = new snapInstrument(10, this);
	    qRegisterMetaType<std::string>();
		//connect(_snapMarketDataLive, &s4SnapMarketDataAgent::signal_L2Data_instrument_snap, pInstrument, &snapInstrument::onL2Data_instrument_snap);
		//connect(_snapMarketDataLive, &s4SnapMarketDataAgent::signal_L2Data_index_snap, pInstrument, &snapInstrument::onL2Data_index_snap);
		//connect(_snapMarketDataLive, &s4SnapMarketDataAgent::signal_L2Data_order, pInstrument, &snapInstrument::onL2Data_order);
		//connect(_snapMarketDataLive, &s4SnapMarketDataAgent::signal_L2Data_exec, pInstrument, &snapInstrument::onL2Data_exec);

		std::string signalName("signal_L2Data_instrument_snap");
		signalName += code.toStdString();
		signalName += "(sharedCharArray_ptr)";
		if (!_snapMarketDataLive->connectDynamicSignal(signalName.data(), pInstrument, "slot_testPtr(sharedCharArray_ptr)")) {
			qDebug() << "connectDynamicSignal fail!";
		}

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
	slot_stopMDSource();
	_snapMarketDataLive->stop();
}

}
}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
