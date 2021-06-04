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
}

void snapInstrument_table::addSnaps(const std::vector<tdx_snap_t>& vSnap)
{
    ((snapInstrument_tableMarket*)_market)->addSnaps(vSnap);
}




} // namespace QT
} // namespace S4
