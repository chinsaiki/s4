#pragma once

#include "qt_Kviewer/s4Kinstrument_view.h"
#include "qt_SnapViewer/s4SnapInstrument_Kline_scene.h"
// #include "qt_common/s4qt_data_if.h"
#include "data/s4infSnap.h"


namespace S4{
namespace QT{
    
class snapInstrument_Kline_view : public Kinstrument_view
{
    Q_OBJECT
public:
    snapInstrument_Kline_view(snapInstrument_Kline_scene* scene, QWidget *parent = 0);
    virtual ~snapInstrument_Kline_view(){}

    void setCtx(const std::shared_ptr<infSnapQ_t>& pInfSnapQ);

    void paint(void);

    virtual void fitView(void) override;

public slots:
    //seq >=0: next; <0: last
	void slot_next_trade(int seq);

protected:
    int _seq = 0;
};

} // namespace QT
} // namespace S4


