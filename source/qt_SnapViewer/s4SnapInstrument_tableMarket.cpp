#include "qt_SnapViewer/s4SnapInstrument_tableMarket.h"
#include "qt_SnapViewer/s4SnapTableModel_snapInfo.h"
#include "qt_SnapViewer/s4SnapTableModel_level.h"

#include <QGridLayout>
#include <QHeaderView>

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
    connect(this, &snapInstrument_tableMarket::signal_L2Data_instrument_snap, levels, &snapTableModel_level::refreshL2);

    _info_tv = new QTableView(this);
    _info_tv->setItemDelegate(delegate);
    snapTableModel_snapInfo* infos = new snapTableModel_snapInfo(_info_tv);
    _info_tv->setModel(infos);
    _info_tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    _info_tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // _info_tv->verticalHeader()->setMaximumHeight(24);
    // _info_tv->verticalHeader()->setMinimumHeight(5);
    _info_tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(this, &snapInstrument_tableMarket::signal_L2Data_instrument_snap, infos, &snapTableModel_snapInfo::refreshL2);

    {
        // const int nNumRows = levels->rowCount(QModelIndex());
        // int nRowHeight = _level_tv->rowHeight(0);
        // int nTableHeight = (nNumRows * nRowHeight) + _level_tv->horizontalHeader()->height() + 2 * _level_tv->frameWidth();
        // _level_tv->setMinimumHeight(nTableHeight);
        // _level_tv->setMaximumHeight(nTableHeight);
        // _level_tv->resizeColumnsToContents();
        // _level_tv->resizeRowsToContents();

    }
    
    {
        // const int nNumRows = infos->rowCount(QModelIndex());
        // int nRowHeight = _info_tv->rowHeight(0);
        // int nTableHeight = (nNumRows * nRowHeight) + _info_tv->horizontalHeader()->height() + 2 * _info_tv->frameWidth();
        // _info_tv->setMinimumHeight(nTableHeight);
        // _info_tv->setMaximumHeight(nTableHeight);
        // _info_tv->resizeColumnsToContents();
        // _info_tv->resizeRowsToContents();
    }

    //网格分割
	QGridLayout *pLayout = new QGridLayout();
	// pLayout->addWidget(_K_tab, 0, 0, 3, 6);			//3x6 row x col
	// pLayout->addWidget(_cyc_tab, 0, 6, 3, 1);		//3x1
	// pLayout->addWidget(_indicator_tab, 3, 0, 1, 6);	//1x6
	// pLayout->addWidget(_basic_tab, 3, 6, 1, 1);		//1x1
    pLayout->addWidget(_level_tv, 0, 0);
    pLayout->addWidget(_info_tv, 20, 0);

	// _cyc_tab->setMaximumWidth(400);
	// _basic_tab->setMaximumWidth(400);

	// pLayout->setRowStretch(0, 3);			//
	// pLayout->setRowStretch(3, 1);			//
	// pLayout->setColumnStretch(0, 3);			//0列的拉伸系数
	// pLayout->setColumnStretch(6, 1);			//6列的拉伸系数 (0=不拉伸)

	setLayout(pLayout);
}


void snapInstrument_tableMarket::addSnaps(const std::vector<tdx_snap_t>& vSnap)
{
	QAbstractItemModel* levels = _level_tv->model();
    ((snapTableModel_level*)levels)->refresh(vSnap.back());
    
	QAbstractItemModel* infos = _info_tv->model();
    ((snapTableModel_snapInfo*)infos)->refresh(vSnap.back());
}

void snapInstrument_tableMarket::onL2Data_instrument_snap(const std::string& s)
{
	emit signal_L2Data_instrument_snap(s);
}
void snapInstrument_tableMarket::onL2Data_index_snap(const std::string& s)
{
	emit signal_L2Data_index_snap(s);
}
void snapInstrument_tableMarket::onL2Data_order(const std::string& s)
{
	emit signal_L2Data_order(s);
}
void snapInstrument_tableMarket::onL2Data_exec(const std::string& s)
{
	emit signal_L2Data_exec(s);
}



} // namespace QT
} // namespace S4
