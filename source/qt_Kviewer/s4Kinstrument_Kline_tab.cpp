#include "qt_Kviewer/s4Kinstrument_Kline_tab.h"

namespace S4{
namespace QT{
    
Kinstrument_Kline_tab::Kinstrument_Kline_tab(QWidget *parent):
    QTabWidget(parent)
{
    setMouseTracking(true);
}


void Kinstrument_Kline_tab::setInstrument(std::shared_ptr<data_panel_t> data_panel){
    _data_panel = data_panel;
    //day
    if (data_panel->info.pDayKQ && data_panel->info.pDayKQ->size()){
        Kinstrument_Kline_scene* _K_scene = new Kinstrument_Kline_scene(this);
        _K_scene->setInfoKQ(data_panel->info.pDayKQ);
        _K_scene->setMAmap(data_panel->info.pMAlib);
        _K_scene->paint();
        Kinstrument_Kline_view* _K_view = new Kinstrument_Kline_view(_K_scene, this);
        _K_view->setCtx(data_panel->info.pDayKQ);
        _K_view->paint();
        int i = addTab(_K_view, "day");
        setCurrentIndex(i);
        _timeMode = tDAY;

        emit paint_indicator(Kinstrument_indicator_scene::ind_type::IND_VOL , timeMode_t::tDAY );
		connect(_K_view, SIGNAL(signalViewEvent(std::shared_ptr<view_event>)), this, SLOT(slotViewEvent_day(std::shared_ptr<view_event>)));
    }
}


void Kinstrument_Kline_tab::slotScaleChanged_day(qreal x_scale, qreal y_scale)
{
	emit signalScaleChanged(x_scale, y_scale, timeMode_t::tDAY);
}

void Kinstrument_Kline_tab::slotSetTransform_day(const QTransform& T, bool c)
{
	emit signalSetTransform(T, c, timeMode_t::tDAY);
}

void Kinstrument_Kline_tab::slotLabelCenterChanged_day(qreal label_x, qreal label_y)
{
	emit signalLabelCenterChanged(label_x, label_y, timeMode_t::tDAY);
}

void Kinstrument_Kline_tab::slotLabelMouseChanged_day(qreal label_x, qreal label_y)
{
	emit signalLabelMouseChanged(label_x, label_y, timeMode_t::tDAY);
}

void Kinstrument_Kline_tab::slotCenterChanged_day(qreal scene_x, qreal scene_y)
{
	emit signalCenterChanged(scene_x, scene_y, timeMode_t::tDAY);
}

void Kinstrument_Kline_tab::slotMouseChanged_day(qreal scene_x, qreal scene_y)
{
	emit signalMouseChanged(scene_x, scene_y, timeMode_t::tDAY);
}

void Kinstrument_Kline_tab::slotViewEvent_day(std::shared_ptr<view_event> event)
{
	event->setTimeMode(timeMode_t::tDAY);
	emit signalViewEvent(event);
}

} // namespace QT
} // namespace S4
