
#include "qt_Kviewer/s4Kinstrument_indicator_view.h"

namespace S4{
namespace QT{

Kinstrument_indicator_view::Kinstrument_indicator_view(Kinstrument_indicator_scene * scene, QWidget *parent):
    Kinstrument_view(scene, parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	ctx_t ctx;
	ctx.sc_val_h_max = scene->getCtx().val_h_max();
	ctx.sc_val_h_min = 0;//scene->getCtx().val_h_min();
	ctx.sc_val_w_max = scene->getCtx().val_w_max();
	ctx.sc_val_w_min = 0;// scene->getCtx().val_w_min();
	setCtx(ctx);
	setGridGap_h(0.25);
}

void Kinstrument_indicator_view::paint(void){
    paintGridLines();
    //paintGridLabels();
    //onViewChange();
	//paintCrosshair();
	//qDebug() << this->transform();
	qreal x_scale = 1;
	qreal height = this->height();
	qreal scene_h = _scene->sceneRect().height();
	qreal y_scale =  height / scene_h;
	QTransform T;
	T.scale(x_scale, y_scale);
	Kinstrument_view::setTransform(T, false);
	//qDebug() << this->transform();
    setIsPaint(true);
}

void Kinstrument_indicator_view::slotSetTransform(const QTransform& Ti, bool combine)
{
	QTransform T;
	qreal x_scale = Ti.m11();
	qreal y_scale = this->height() / _scene->sceneRect().height();
	T.scale(x_scale, y_scale);
	Kinstrument_view::setTransform(T, combine);
}

void Kinstrument_indicator_view::slotMouseChanged(qreal scene_x, qreal scene_y)
{
	if (!_scene)
		return;

	QPointF view_mouse_pos;
	bool ret = _scene->get_valPos(_scene->x_to_val_w(scene_x)+0.5, view_mouse_pos);
	if (ret){
		view_mouse_pos.setX(_scene->val_w_to_x(view_mouse_pos.x()));
		view_mouse_pos.setY(_scene->val_h_to_y(view_mouse_pos.y()));
		view_mouse_pos = mapFromScene(view_mouse_pos);
	}else{
		view_mouse_pos = mapFromScene(scene_x, scene_y);
	}
	onMouseChange(view_mouse_pos);

	paintCrosshair();
}

//void Kinstrument_indicator_view::onScaleChanged(qreal x_scale, qreal y_scale)
//{
//	qreal _y_scale = height() / _scene->sceneRect().height();
//	scale(x_scale, _y_scale);
//	onViewChange();
//}

void Kinstrument_indicator_view::slotLabelCenterChanged(qreal label_x, qreal label_y)
{
	if(!_scene)
		return;

	qreal x = _scene->label_w_to_x(label_x);
	qreal y = mapToScene(0, height()/2).y();
	centerOn(x, y);
	onViewChange();

	QPointF view_mouse_pos = mapFromScene(x, y);
	onMouseChange(view_mouse_pos);
}

void Kinstrument_indicator_view::slotViewEvent(std::shared_ptr<view_event> event)
{
	if (_timeMode != event->timeMode())
		return;

	Kinstrument_view::slotViewEvent(event);
}

} // namespace QT
} // namespace S4


