
#include "qt_Kviewer/s4Kinstrument_Kline_view.h"

namespace S4{
namespace QT{

Kinstrument_Kline_view::Kinstrument_Kline_view(Kinstrument_Kline_scene* scene, QWidget *parent):
    Kinstrument_view(scene, parent)
{

}

void Kinstrument_Kline_view::setCtx(const std::shared_ptr<infKQ_t>& pInfoKQ){
    if (!pInfoKQ || !pInfoKQ->size())
        return;

    if (!pInfoKQ->isNewAtBack())
        return;

    ctx_t ctx;
    ctx.set_val_h_max(pInfoKQ->front()->high);
    ctx.set_val_h_min(pInfoKQ->front()->low);
    ctx.set_val_w_max(0);
    ctx.set_val_w_min(0);

    int n = 0;
    for(const auto& d : *pInfoKQ)
    {
        if (d->high > ctx.val_h_max()){
            ctx.set_val_h_max(d->high);
        }
        if (d->low < ctx.val_h_min()){
            ctx.set_val_h_min(d->low);
        }
        ctx.set_val_w_max(n);
        n++;
    }

    Kinstrument_view::setCtx(ctx);
}

void Kinstrument_Kline_view::fitView(void)
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


void Kinstrument_Kline_view::paint(void){
    paintGridLines();
    //paintGridLabels();
    //onViewChange();
    //paintCrosshair();
    setIsPaint(true);
}

void Kinstrument_Kline_view::slot_next_trade(int next)
{

    QPointF trad_valPos;
    if (!_scene || !((Kinstrument_Kline_scene*)_scene)->get_trade_valPos(_seq, trad_valPos)){
        return;
    }

	qreal scene_x = _scene->val_w_to_x(trad_valPos.x());
	qreal scene_y = _scene->val_h_to_y(trad_valPos.y());

	centerOn(scene_x, scene_y);
    onViewChange();
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>(scene_x, scene_y);
	emit signalViewEvent(e_center);

    //TODO: jump more scope?
    if (next >= 0) {
        _seq++;
    }
    else {
        _seq--;
    }
    
}


} // namespace QT
} // namespace S4


