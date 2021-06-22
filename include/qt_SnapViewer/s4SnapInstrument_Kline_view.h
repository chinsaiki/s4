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

    void paint(const infSnapQ_ptr& pSnaps);

    virtual void fitView(void) override;

public slots:
    

signals:
    void signal_mouseSnapTime(time_t time);

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override;

    virtual void paintCrosshair() override;

	void setCtx(const std::shared_ptr<infSnapQ_t>& pInfSnapQ);
};

} // namespace QT
} // namespace S4


