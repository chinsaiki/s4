﻿#include "qt_SnapViewer/s4SnapInstrument_tableMarket.h"
#include "qt_SnapViewer/s4SnapTableModel_snapInfo.h"
#include "qt_SnapViewer/s4SnapTableModel_level.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QSplitter>
#include <QLineEdit>
#include <QLabel>

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
    _info_tv->horizontalHeader()->setVisible(false);
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
    

	//整体布局
	QSplitter* splitter = new QSplitter(Qt::Orientation::Vertical, this);
	splitter->addWidget(_level_tv);
	splitter->addWidget(_info_tv);

    splitter->setSizes({80, 50});

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

void snapInstrument_tableMarket::onL2Data_instrument_snap(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_instrument_snap(s);
}
void snapInstrument_tableMarket::onL2Data_index_snap(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_index_snap(s);
}

void snapInstrument_tableMarket::onL2Data_order(const S4::sharedCharArray_ptr& s)
{
    _order_cnt++;
    _order_info->setText(QStringLiteral("接收数量：") + QString::number(_order_cnt));
	emit signal_L2Data_order(s);
}
void snapInstrument_tableMarket::onL2Data_exec(const S4::sharedCharArray_ptr& s)
{
    _exec_cnt++;
    _exec_info->setText(QStringLiteral("接收数量：") + QString::number(_exec_cnt));
	emit signal_L2Data_exec(s);
}



} // namespace QT
} // namespace S4
