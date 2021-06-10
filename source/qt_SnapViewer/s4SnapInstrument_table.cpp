#include "qt_SnapViewer/s4SnapInstrument_table.h"
#include "qt_SnapViewer/s4SnapInstrument_tableMarket.h"
#include "qt_SnapViewer/s4SnapInstrument_tableBasic.h"
namespace S4{
namespace QT{
    
snapInstrument_table::snapInstrument_table(int snapLeves_nb, QWidget *parent):
    QTabWidget(parent)
{
    setMouseTracking(true);

    _market = new snapInstrument_tableMarket(snapLeves_nb, this);
    _basic = new snapInstrument_tableBasic(this);

	addTab(_market, "market");
	addTab(_basic, "basic");
	setCurrentIndex(0);

	connect(this, &snapInstrument_table::signal_L2Data_instrument_snap, (snapInstrument_tableMarket*)_market, &snapInstrument_tableMarket::onL2Data_instrument_snap);
	connect(this, &snapInstrument_table::signal_L2Data_index_snap, (snapInstrument_tableMarket*)_market, &snapInstrument_tableMarket::onL2Data_index_snap);
	connect(this, &snapInstrument_table::signal_L2Data_order, (snapInstrument_tableMarket*)_market, &snapInstrument_tableMarket::onL2Data_order);
	connect(this, &snapInstrument_table::signal_L2Data_exec, (snapInstrument_tableMarket*)_market, &snapInstrument_tableMarket::onL2Data_exec);
}

void snapInstrument_table::addSnaps(const std::vector<tdx_snap_t>& vSnap)
{
    ((snapInstrument_tableMarket*)_market)->addSnaps(vSnap);
}

void snapInstrument_table::onL2Data_instrument_snap(const std::string& s)
{
	emit signal_L2Data_instrument_snap(s);
}
void snapInstrument_table::onL2Data_index_snap(const std::string& s)
{
	emit signal_L2Data_index_snap(s);
}
void snapInstrument_table::onL2Data_order(const sharedCharArray_ptr& s)
{
	emit signal_L2Data_order(s);
}
void snapInstrument_table::onL2Data_exec(const std::string& s)
{
	emit signal_L2Data_exec(s);
}




} // namespace QT
} // namespace S4
