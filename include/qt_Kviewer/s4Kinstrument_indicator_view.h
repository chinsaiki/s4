#pragma once

#include "qt_Kviewer/s4Kinstrument_view.h"
#include "qt_Kviewer/s4Kinstrument_indicator_scene.h"
#include "qt_common/s4qt_data_if.h"


namespace S4{
namespace QT{
    
class Kinstrument_indicator_view : public Kinstrument_view
{
public:
    Kinstrument_indicator_view(Kinstrument_indicator_scene *scene, QWidget *parent = 0);
    virtual ~Kinstrument_indicator_view(){}


    void paint(void);


	inline void setTimeMode(timeMode_t t) {
		_timeMode = t;
	}

	inline timeMode_t timeMode(void) const {
		return _timeMode;
	}


public slots:
    //virtual void onScaleChanged(qreal x_scale, qreal y_scale) override;
    virtual void slotLabelCenterChanged(qreal label_x, qreal label_y) override;
	virtual void slotSetTransform(const QTransform&, bool) override;
	virtual void slotMouseChanged(qreal scene_x, qreal scene_y) override;
	virtual void slotViewEvent(std::shared_ptr<view_event>) override;

protected:
	timeMode_t _timeMode;

};

} // namespace QT
} // namespace S4


