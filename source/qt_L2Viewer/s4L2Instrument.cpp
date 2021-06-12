#include "qt_L2Viewer/s4L2Instrument.h"
#include "qt_L2Viewer/s4L2Instrument_table.h"
#include <QGridLayout>

namespace S4{
namespace QT{
    
L2Instrument::L2Instrument(int snapLevels_nb, QWidget *parent) :
    QWidget(parent)
{
	this->setMouseTracking(true);
	//int i;
	// _K_tab = new SnapInstrument_Kline_tab(this);

	// _indicator_tab = new SnapInstrument_indicator_tab(this);

	// connect(_K_tab, SIGNAL(paint_indicator(SnapInstrument_indicator_scene::ind_type, timeMode_t)),
	// 	_indicator_tab, SLOT(paint(SnapInstrument_indicator_scene::ind_type, timeMode_t)));

	// connect(_K_tab, SIGNAL(signalViewEvent(std::shared_ptr<view_event>)), _indicator_tab, SLOT(slotViewEvent(std::shared_ptr<view_event>)));

    _table = new L2Instrument_table(snapLevels_nb, this);
	connect(this, &L2Instrument::signal_L2Data_instrument_snap, (L2Instrument_table*)_table, &L2Instrument_table::onL2Data_instrument_snap);
	connect(this, &L2Instrument::signal_L2Data_index_snap, (L2Instrument_table*)_table, &L2Instrument_table::onL2Data_index_snap);
	connect(this, &L2Instrument::signal_L2Data_order, (L2Instrument_table*)_table, &L2Instrument_table::onL2Data_order);
	connect(this, &L2Instrument::signal_L2Data_exec, (L2Instrument_table*)_table, &L2Instrument_table::onL2Data_exec);
    
	//网格分割
	QGridLayout *pLayout = new QGridLayout();
    pLayout->addWidget(_table, 0, 0);

	setLayout(pLayout);
}


void L2Instrument::onL2Data_instrument_snap(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_instrument_snap(s);
}
void L2Instrument::onL2Data_index_snap(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_index_snap(s);
}
void L2Instrument::onL2Data_order(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_order(s);
}
void L2Instrument::onL2Data_exec(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_exec(s);
}


} // namespace QT
} // namespace S4
