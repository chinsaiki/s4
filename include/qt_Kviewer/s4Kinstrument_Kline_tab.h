#pragma once

#include "qt_Kviewer/s4Kinstrument_Kline_view.h"
#include "qt_Kviewer/s4Kinstrument_Kline_scene.h"
#include "qt_Kviewer/s4Kinstrument_indicator_scene.h"
#include "qt_common/s4qt_data_if.h"

#include <QTabWidget>
#include <QKeyEvent>

namespace S4{
namespace QT{
    
class Kinstrument_Kline_tab: public QTabWidget
{
    Q_OBJECT
public:
    explicit Kinstrument_Kline_tab(QWidget *parent = nullptr);
    virtual ~Kinstrument_Kline_tab(){}

    void setInstrument(std::shared_ptr<data_panel_t> data_panel);

signals:
    void paint_indicator(Kinstrument_indicator_scene::ind_type , timeMode_t );


signals:
	void signalScaleChanged(qreal, qreal, timeMode_t);
	void signalSetTransform(const QTransform&, bool, timeMode_t);
	void signalLabelCenterChanged(qreal, qreal, timeMode_t);
	void signalLabelMouseChanged(qreal, qreal, timeMode_t);
	void signalCenterChanged(qreal, qreal, timeMode_t);
	void signalMouseChanged(qreal, qreal, timeMode_t);

	void signalViewEvent(std::shared_ptr<view_event>);

public slots:
	virtual void slotScaleChanged_day(qreal x_scale, qreal y_scale);
	virtual void slotSetTransform_day(const QTransform&, bool);
	virtual void slotLabelCenterChanged_day(qreal label_x, qreal label_y);
	virtual void slotLabelMouseChanged_day(qreal label_x, qreal label_y);
	virtual void slotCenterChanged_day(qreal scene_x, qreal scene_y);
	virtual void slotMouseChanged_day(qreal scene_x, qreal scene_y);

	virtual void slotViewEvent_day(std::shared_ptr<view_event>);

public slots:
	//seq >=0: next; <0: last
	void slot_next_trade(int seq);

protected:
    std::shared_ptr<data_panel_t> _data_panel;
    timeMode_t _timeMode = timeMode_t::tUNKNOWN;
};


} // namespace QT
} // namespace S4
