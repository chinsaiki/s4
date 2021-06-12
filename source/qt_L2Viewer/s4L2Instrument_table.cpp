#include "qt_L2Viewer/s4L2Instrument_table.h"
#include "qt_L2Viewer/s4L2Instrument_tableMarket.h"
namespace S4{
namespace QT{
    
L2Instrument_table::L2Instrument_table(int snapLeves_nb, QWidget *parent):
    QTabWidget(parent)
{
    setMouseTracking(true);

    _market = new L2Instrument_tableMarket(snapLeves_nb, this);

	addTab(_market, QStringLiteral("L2行情数据"));
	setCurrentIndex(0);

	connect(this, &L2Instrument_table::signal_L2Data_instrument_snap, (L2Instrument_tableMarket*)_market, &L2Instrument_tableMarket::onL2Data_instrument_snap);
	connect(this, &L2Instrument_table::signal_L2Data_index_snap, (L2Instrument_tableMarket*)_market, &L2Instrument_tableMarket::onL2Data_index_snap);
	connect(this, &L2Instrument_table::signal_L2Data_order, (L2Instrument_tableMarket*)_market, &L2Instrument_tableMarket::onL2Data_order);
	connect(this, &L2Instrument_table::signal_L2Data_exec, (L2Instrument_tableMarket*)_market, &L2Instrument_tableMarket::onL2Data_exec);
}


void L2Instrument_table::onL2Data_instrument_snap(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_instrument_snap(s);
}
void L2Instrument_table::onL2Data_index_snap(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_index_snap(s);
}
void L2Instrument_table::onL2Data_order(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_order(s);
}
void L2Instrument_table::onL2Data_exec(const S4::sharedCharArray_ptr& s)
{
	emit signal_L2Data_exec(s);
}




} // namespace QT
} // namespace S4
