#include "qt_SnapViewer/s4SnapInstrument.h"
#include "qt_SnapViewer/s4SnapInstrument_table.h"
#include <QGridLayout>

namespace S4{
namespace QT{
    
snapInstrument::snapInstrument(int snapLevels_nb, QWidget *parent) :
    QWidget(parent)
{
	this->setMouseTracking(true);
	//int i;
	// _K_tab = new SnapInstrument_Kline_tab(this);

	// _indicator_tab = new SnapInstrument_indicator_tab(this);

	// connect(_K_tab, SIGNAL(paint_indicator(SnapInstrument_indicator_scene::ind_type, timeMode_t)),
	// 	_indicator_tab, SLOT(paint(SnapInstrument_indicator_scene::ind_type, timeMode_t)));

	// connect(_K_tab, SIGNAL(signalViewEvent(std::shared_ptr<view_event>)), _indicator_tab, SLOT(slotViewEvent(std::shared_ptr<view_event>)));

    _table = new snapInstrument_table(snapLevels_nb, this);
	connect(this, &snapInstrument::signal_L2Data_instrument_snap, (snapInstrument_table*)_table, &snapInstrument_table::onL2Data_instrument_snap);
	connect(this, &snapInstrument::signal_L2Data_index_snap, (snapInstrument_table*)_table, &snapInstrument_table::onL2Data_index_snap);
	connect(this, &snapInstrument::signal_L2Data_order, (snapInstrument_table*)_table, &snapInstrument_table::onL2Data_order);
	connect(this, &snapInstrument::signal_L2Data_exec, (snapInstrument_table*)_table, &snapInstrument_table::onL2Data_exec);
    
	//网格分割
	QGridLayout *pLayout = new QGridLayout();
	// pLayout->addWidget(_K_tab, 0, 0, 3, 6);			//3x6 row x col
	// pLayout->addWidget(_cyc_tab, 0, 6, 3, 1);		//3x1
	// pLayout->addWidget(_indicator_tab, 3, 0, 1, 6);	//1x6
	// pLayout->addWidget(_basic_tab, 3, 6, 1, 1);		//1x1
    pLayout->addWidget(_table, 0, 0);

	// _cyc_tab->setMaximumWidth(400);
	// _basic_tab->setMaximumWidth(400);

	// pLayout->setRowStretch(0, 3);			//
	// pLayout->setRowStretch(3, 1);			//
	// pLayout->setColumnStretch(0, 3);			//0列的拉伸系数
	// pLayout->setColumnStretch(6, 1);			//6列的拉伸系数 (0=不拉伸)

	setLayout(pLayout);
}

void snapInstrument::addSnaps(const std::vector<tdx_snap_t>& vSnap)
{
    ((snapInstrument_table*)_table)->addSnaps(vSnap);
}

// void snapInstrument::setInstrument(const data_panel_t& data_panel){
// 	_data_panel = std::make_shared<data_panel_t>(data_panel);
// 	_indicator_tab->setInstrument(_data_panel);

// 	_K_tab->setInstrument(_data_panel);

// }
void snapInstrument::slot_testPtr(const sharedCharArray_ptr& ptr)
{
	qDebug() << ptr->curRef();
}

void snapInstrument::onL2Data_instrument_snap(const std::string& s)
{
	emit signal_L2Data_instrument_snap(s);
}
void snapInstrument::onL2Data_index_snap(const std::string& s)
{
	emit signal_L2Data_index_snap(s);
}
void snapInstrument::onL2Data_order(const sharedCharArray_ptr& s)
{
	emit signal_L2Data_order(s);
}
void snapInstrument::onL2Data_exec(const std::string& s)
{
	emit signal_L2Data_exec(s);
}


} // namespace QT
} // namespace S4
