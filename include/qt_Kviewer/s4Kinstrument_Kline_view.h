#pragma once

#include "qt_Kviewer/s4Kinstrument_view.h"
#include "qt_Kviewer/s4Kinstrument_Kline_scene.h"
#include "qt_common/s4qt_data_if.h"


namespace S4{
namespace QT{
    
class Kinstrument_Kline_view : public Kinstrument_view
{
    Q_OBJECT
public:
    Kinstrument_Kline_view(Kinstrument_Kline_scene* scene, QWidget *parent = 0);
    virtual ~Kinstrument_Kline_view(){}

    void setCtx(const std::shared_ptr<infKQ_t>& pInfoKQ);

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


