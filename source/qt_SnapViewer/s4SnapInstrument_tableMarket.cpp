#include "qt_SnapViewer/s4SnapInstrument_tableMarket.h"
#include "qt_SnapViewer/s4SnapTableModel_snapInfo.h"
#include "qt_SnapViewer/s4SnapTableModel_level.h"
#include "qt_SnapViewer/s4SnapTableModel_order.h"
#include "qt_SnapViewer/s4SnapTableModel_exec.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QSplitter>

namespace S4{
namespace QT{
    
snapInstrument_tableMarket::snapInstrument_tableMarket(int snapLeves_nb, QWidget *parent):
    QWidget(parent)
{
    setMouseTracking(true);
    itemFormatDelegate* delegate = new itemFormatDelegate(this);

    _level_tv = new QTableView(this);
    _level_tv->setItemDelegate(delegate);
    snapTableModel_level* levels = new snapTableModel_level(snapLeves_nb, _level_tv);
    _level_tv->setModel(levels);
    _level_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _level_tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // _level_tv->verticalHeader()->setMaximumHeight(24);
    // _level_tv->verticalHeader()->setMinimumHeight(5);
    _level_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
	_level_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);	//限制选择
	_level_tv->setMaximumWidth(250);
    connect(this, &snapInstrument_tableMarket::signal_L2Data_instrument_snap, levels, &snapTableModel_level::refreshL2);
    {
        const int nNumRows = levels->rowCount(QModelIndex());
        _level_tv->resizeRowsToContents();
        _level_tv->resizeColumnsToContents();
        int nRowHeight = _level_tv->rowHeight(0);
        int nTableHeight = (nNumRows * nRowHeight) + _level_tv->horizontalHeader()->height() + 2 * _level_tv->frameWidth();
        _level_tv->setMinimumHeight(nTableHeight);
        _level_tv->setMaximumHeight(nTableHeight);

    }

    _info_tv = new QTableView(this);
    _info_tv->setItemDelegate(delegate);
    snapTableModel_snapInfo* infos = new snapTableModel_snapInfo(_info_tv);
    _info_tv->setModel(infos);
    _info_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _info_tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // _info_tv->verticalHeader()->setMaximumHeight(24);
    // _info_tv->verticalHeader()->setMinimumHeight(5);
    _info_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
	_info_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);	//限制选择
	_info_tv->setMaximumWidth(250);
    connect(this, &snapInstrument_tableMarket::signal_L2Data_instrument_snap, infos, &snapTableModel_snapInfo::refreshL2);
    {
        const int nNumRows = infos->rowCount(QModelIndex());
        _info_tv->resizeRowsToContents();
        _info_tv->resizeColumnsToContents();
        int nRowHeight = _info_tv->rowHeight(0);
        int nTableHeight = (nNumRows * nRowHeight) + _info_tv->horizontalHeader()->height() + 2 * _info_tv->frameWidth();
        _info_tv->setMinimumHeight(nTableHeight);
        _info_tv->setMaximumHeight(nTableHeight);
    }
    
    _order_tv = new QTableView(this);
    _order_tv->setItemDelegate(delegate);
    snapTableModel_order* orders = new snapTableModel_order(_order_tv);
    _order_tv->setModel(orders);
    _order_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _order_tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // _order_tv->verticalHeader()->setMaximumHeight(24);
    // _order_tv->verticalHeader()->setMinimumHeight(5);
    _order_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
	_order_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);	//限制选择
	_order_tv->setMaximumWidth(750);
    connect(this, &snapInstrument_tableMarket::signal_L2Data_order, orders, &snapTableModel_order::refreshL2);

	_exec_tv = new QTableView(this);
	_exec_tv->setItemDelegate(delegate);
	snapTableModel_exec* execs = new snapTableModel_exec(_exec_tv);
	_exec_tv->setModel(execs);
	_exec_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	_exec_tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// _exec_tv->verticalHeader()->setMaximumHeight(24);
	// _exec_tv->verticalHeader()->setMinimumHeight(5);
	_exec_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
	_exec_tv->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);	//限制选择
	_exec_tv->setMaximumWidth(750);
	connect(this, &snapInstrument_tableMarket::signal_L2Data_exec, execs, &snapTableModel_exec::refreshL2);

	//整体布局
	QSplitter* splitter = new QSplitter(this);	//横排
	splitter->addWidget(_level_tv);
	splitter->addWidget(_info_tv);
	QSplitter* splitterV = new QSplitter(Qt::Orientation::Vertical, this);	//竖排
	splitterV->addWidget(_order_tv);
	splitterV->addWidget(_exec_tv);
	splitterV->setSizes({ 150, 150 });
	splitter->addWidget(splitterV);
    splitter->setSizes({50, 50, 150});

    //布局放进网格，使填充满
	QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(splitter);
	setLayout(pLayout);

}


void snapInstrument_tableMarket::addSnaps(const std::vector<tdx_snap_t>& vSnap)
{
	QAbstractItemModel* levels = _level_tv->model();
    ((snapTableModel_level*)levels)->refresh(vSnap.back());
    
	QAbstractItemModel* infos = _info_tv->model();
    ((snapTableModel_snapInfo*)infos)->refresh(vSnap.back());
}

void snapInstrument_tableMarket::onL2Data_instrument_snap(const sharedCharArray_ptr& s)
{
	emit signal_L2Data_instrument_snap(s);
}
void snapInstrument_tableMarket::onL2Data_index_snap(const sharedCharArray_ptr& s)
{
	emit signal_L2Data_index_snap(s);
}
void snapInstrument_tableMarket::onL2Data_order(const sharedCharArray_ptr& s)
{
	emit signal_L2Data_order(s);
}
void snapInstrument_tableMarket::onL2Data_exec(const sharedCharArray_ptr& s)
{
	emit signal_L2Data_exec(s);
}



} // namespace QT
} // namespace S4
