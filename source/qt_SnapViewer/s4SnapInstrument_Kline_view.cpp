
#include "qt_SnapViewer/s4SnapInstrument_Kline_view.h"

namespace S4{
namespace QT{

snapInstrument_Kline_view::snapInstrument_Kline_view(snapInstrument_Kline_scene* scene, QWidget *parent):
    Kinstrument_view(scene, parent)
{

}


void snapInstrument_Kline_view::setCtx(const std::shared_ptr<infSnapQ_t>& pInfSnapQ)
{
    if (!pInfSnapQ || !pInfSnapQ->size())
        return;

    if (!pInfSnapQ->isNewAtBack())
        return;

    ctx_t ctx;
    // ctx.set_val_h_max(pInfSnapQ->front()->high);
    // ctx.set_val_h_min(pInfSnapQ->front()->low);
    ctx.set_val_h_max(pInfSnapQ->front()->up20P());
    ctx.set_val_h_min(pInfSnapQ->front()->dn20P());
    ctx.set_val_w_max(0);
    ctx.set_val_w_min(0);

    const time_minuSec_t KTP_BGN_0 = KTP_STK_PBREAK + 100;
    const time_minuSec_t KTP_END_0 = KTP_STK_TRADE1 - 100;
    const int KTM_0 = KTM_STK_PBREAK - 2;

    const time_minuSec_t KTP_BGN_1 = KTP_STK_BREAK + 100;
    const time_minuSec_t KTP_END_1 = KTP_STK_TRADE2 - 100;
    const int KTM_1 = KTM_STK_BREAK - 2;

    time_t bgn_time = pInfSnapQ->front()->_time;

    if (pInfSnapQ->front()->_MinmuSec > KTP_END_0)
        bgn_time -= KTM_0 * 60;
    if (pInfSnapQ->front()->_MinmuSec > KTP_END_1)
        bgn_time -= KTM_1 * 60;

    time_t dlt_time = 0;
    for(const auto& d : *pInfSnapQ)
    {
        dlt_time = d->_time - bgn_time;
        if (d->_MinmuSec >= KTP_BGN_0 && d->_MinmuSec <= KTP_END_0) continue;
        if (d->_MinmuSec >= KTP_BGN_1 && d->_MinmuSec <= KTP_END_1) continue;
        if (d->_MinmuSec > KTP_END_0)
            dlt_time -= KTM_0 * 60;
        if (d->_MinmuSec > KTP_END_1)
            dlt_time -= KTM_1 * 60;

    }
    ctx.set_val_w_max(dlt_time);

    Kinstrument_view::setCtx(ctx);
}

void snapInstrument_Kline_view::fitView(void)
{
    QPointF valPos;
    bool valid = _scene->get_valPos(-1, valPos);
    if (!valid){
        Kinstrument_view::fitView();
        return;
    }
    
	centerOn(_scene->val_w_to_x(valPos.x()), _scene->val_h_to_y(valPos.y()));
	onViewChange();
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>((_scene_lu + _scene_rd) / 2);
	emit signalViewEvent(e_center);

    //TODO: not useful ??
	//std::shared_ptr<view_event_transform_change> e_trans = std::make_shared<view_event_transform_change>(this->transform(), false);
	//emit signalViewEvent(e_trans);
}


void snapInstrument_Kline_view::paint(const infSnapQ_ptr& pSnaps){
	setCtx(pSnaps);
	setGridGap_w(60);
	setGridGap_h(0.02);
    paintGridLines();
    //paintGridLabels();
    //onViewChange();
    //paintCrosshair();
    setIsPaint(true);
}

void snapInstrument_Kline_view::mouseMoveEvent(QMouseEvent* event)
{
    Kinstrument_view::mouseMoveEvent(event);

	if (!isPaint()) return;
    if (!_scene){
        return;
    }
    QPointF scene_label_pos;
    QPointF label = ((snapInstrument_Kline_scene*)_scene)->get_label_near(_scene_mouse, scene_label_pos);
    emit signal_mouseSnapTime((time_t)label.x());
}


void snapInstrument_Kline_view::paintCrosshair()
{
    
	if(_crossLine){
		_scene->removeItem(_crossLine);
	}
    QPointF scene_label_pos;
    QPointF label = ((snapInstrument_Kline_scene*)_scene)->get_label_near(_scene_mouse, scene_label_pos);
	_crossLine = paintCrosshairAt(scene_label_pos);
}



} // namespace QT
} // namespace S4


