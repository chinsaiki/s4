#include "qt_Kviewer/s4Kinstrument_view.h"
#include <QDebug>
#include <QGraphicsLineItem>
#include <QtCore/qmath.h>
#include <QScrollBar>
#include "qt_Kviewer/s4Klabel.h"

namespace S4 {
namespace QT {

#define VIEW_Z 100 //TODO:global configure

static const qreal zoom_rate = 1.08;
static const QPointF zoom_fix = { -10, -9 };

Kinstrument_view::Kinstrument_view(Kinstrument_scene*scene, QWidget *parent):
    QGraphicsView(scene, parent),
	_scene(scene)
{
	//TODO: scrollBar not always On
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	//connect(this->verticalScrollBar(), SIGNAL(sliderReleased()),
	//	this, SLOT(verticalScrollvalueChanged()));
	//connect(this->horizontalScrollBar(), SIGNAL(sliderReleased()),
	//	this, SLOT(horizontalScrollvalueChanged()));

	connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(verticalScrollvalueChanged(int)));
	connect(this->horizontalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(horizontalScrollvalueChanged(int)));

    _colorpalette = std::make_shared<qt_colorpalette_t>();

	this->setMouseTracking(true);

	// setCtx_test();
}

//Kinstrument_view::void cursorPosition(QPointF);

qreal Kinstrument_view::val_to_sceneh(qreal val)
{
	qreal y_o;
	if (!_isLogCoor) {
		qreal p_gap = _scene->height() / (_scene->getCtx().val_h_max() - _scene->getCtx().val_h_min());
		y_o = p_gap * (val - _scene->getCtx().val_h_min()) + _scene->sceneRect().y();
	}
	else {
		qreal p_max_h = qLn(_scene->getCtx().val_h_max() / _scene->getCtx().val_h_min()) / qLn(1.0 + _grid_h_gap);
		p_max_h = _scene->height() / p_max_h;
		y_o = qLn(val / _scene->getCtx().val_h_min()) / qLn(1.0 + _grid_h_gap) * p_max_h;
	}
	return y_o;
}

qreal Kinstrument_view::sceneh_to_val(qreal h)
{
	if (!_isLogCoor) {
		return (_scene->getCtx().val_h_max() - _scene->getCtx().val_h_min()) * h / _scene->height() + _scene->getCtx().val_h_min();
	}
	else
	{
		qreal coor = h * qLn(_scene->getCtx().val_h_max() / _scene->getCtx().val_h_min()) / _scene->height() / qLn(1.0 + _grid_h_gap);
		qreal ex = qExp(coor * qLn(1.0 + _grid_h_gap));
		return ex * _scene->getCtx().val_h_min();
	}
}

void Kinstrument_view::mouseDoubleClickEvent(QMouseEvent* event)
{
	//switch drag mode
	if (dragMode() == QGraphicsView::DragMode::ScrollHandDrag) {
		setDragMode(QGraphicsView::DragMode::NoDrag);
	}
	else {
		setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
	}
}

void Kinstrument_view::dragEnterEvent(QDragEnterEvent* event)
{
	qDebug() << event;
}
void Kinstrument_view::dragLeaveEvent(QDragLeaveEvent* event)
{
	qDebug() << event;
}
void Kinstrument_view::dragMoveEvent(QDragMoveEvent* event)
{
	qDebug() << event;
}


void Kinstrument_view::mousePressEvent(QMouseEvent* event)
{
	if (!isPaint()) return;
	if (event->button() == Qt::LeftButton) {
		//_mouse_item = _scene->itemAt(_scene_mouse, transform());
		//if (_mouse_item && !_mouse_item->isSelected()) {
		//	_mouse_item->setSelected(true);
		//	qDebug() << _mouse_item->pos() << _mouse_item->boundingRect();
		//}

		//qDebug() << "There are" << items(event->pos()).size()
		//	<< "items at position" << mapToScene(event->pos());
		//for (auto& i : items(event->pos())) {
		//	i->setSelected(true);
		//	qDebug() << i->pos() << i->boundingRect();
		//}
		if (dragMode() == QGraphicsView::DragMode::ScrollHandDrag) {
			_mouse_press_bgn_pos = _scene_mouse;
			_mouse_press_bgn_center = (_scene_lu + _scene_rd) / 2;
			_drag_to_move = true;
		}
	}
	QGraphicsView::mousePressEvent(event);

}

void Kinstrument_view::mouseReleaseEvent(QMouseEvent* event)
{
	if (!isPaint()) return;
	if (event->button() == Qt::LeftButton) {
		//if (_mouse_item) {
		//	_mouse_item->setSelected(false);
		//}
		if (dragMode() == QGraphicsView::DragMode::ScrollHandDrag) {
			_drag_to_move = false;

		}
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void Kinstrument_view::mouseMoveEvent(QMouseEvent* event)
{
	if (!isPaint()) return;
	onMouseChange(event->pos());
	std::shared_ptr<view_event_scene_mouse_change> e_mouse = std::make_shared<view_event_scene_mouse_change>(_scene_mouse.x(), _scene_mouse.y());
	emit signalViewEvent(e_mouse);

	//_scene_rd -= QPointF(SCROLLBAR_WIDTH, SCROLLBAR_WIDTH);//scrollBar size
	if (_drag_to_move && dragMode() == QGraphicsView::DragMode::ScrollHandDrag) {

		_mouse_press_end_pos = _scene_mouse;
		QPointF movement = _mouse_press_end_pos - _mouse_press_bgn_pos;
		//qDebug() << movement;
		QPointF now_center = _mouse_press_bgn_center - movement;
		
		centerOn(now_center);
		onViewChange();

		_mouse_press_bgn_pos = _scene_mouse - movement;
		_mouse_press_bgn_center = (_scene_lu + _scene_rd) / 2;


		std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>(_mouse_press_bgn_center);
		emit signalViewEvent(e_center);
	}

	paintCrosshair();

}

//resize, scrollBar slid
void Kinstrument_view::onViewChange(void)
{
	if (!isPaint()) return;
	qreal w = width();
	qreal h = height();
	qreal vbw = verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0;
	qreal hbw = horizontalScrollBar()->isVisible() ? horizontalScrollBar()->height() : 0;
	w -= vbw;
	h -= hbw;
	_scene_lu = QGraphicsView::mapToScene(0, 0);
	_scene_rd = QGraphicsView::mapToScene(w - 1, h - 1);
	paintGridLabels();
}

void Kinstrument_view::onMouseChange(qreal view_mouse_x, qreal view_mouse_y)
{
	if (!isPaint()) return;
	_view_mouse_pos.setX(view_mouse_x);
	_view_mouse_pos.setY(view_mouse_y);
	_scene_mouse = QGraphicsView::mapToScene(view_mouse_x, view_mouse_y);
}
void Kinstrument_view::onMouseChange(const QPointF& view_mouse)
{
	if (!isPaint()) return;
	onMouseChange(view_mouse.x(), view_mouse.y());
}

void Kinstrument_view::resizeEvent(QResizeEvent* event)
{
	if (!isPaint()) return;
	onViewChange();
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>((_scene_lu + _scene_rd) / 2);
	emit signalViewEvent(e_center);
}

void Kinstrument_view::wheelEvent(QWheelEvent* event)
{
	if (!isPaint()) return;
	//qDebug() << "Delta: " << event->angleDelta();
	onMouseChange(event->pos());
	std::shared_ptr<view_event_scene_mouse_change> e_mouse = std::make_shared<view_event_scene_mouse_change>(_scene_mouse.x(), _scene_mouse.y());
	emit signalViewEvent(e_mouse);

	int angle = event->angleDelta().y();

	if (angle < 0)
		zoomOut();
	else
		zoomIn();

	onViewChange();
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>((_scene_lu + _scene_rd) / 2);
	emit signalViewEvent(e_center);

	paintCrosshair();
}

//bool Kinstrument_view::viewportEvent(QEvent* e)
//{
//
//	qDebug() << "-" << e->type();
//	return QGraphicsView::viewportEvent(e);
//}
//
//void Kinstrument_view::paintEvent(QPaintEvent*e)
//{
//	QGraphicsView::paintEvent(e);
//	qDebug() << "--" << e->type();
//}


void Kinstrument_view::verticalScrollvalueChanged(int)
{
	if (!isPaint()) return;
	//int value = this->verticalScrollBar()->value();
	//qDebug() <<"verticalScrollvalueChanged"<< value;
	onViewChange();
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>((_scene_lu + _scene_rd) / 2);
	emit signalViewEvent(e_center);
}
void Kinstrument_view::horizontalScrollvalueChanged(int)
{
	if (!isPaint()) return;
	//int value = this->horizontalScrollBar()->value();
	//qDebug() << "horizontalScrollvalueChanged" << value;
	onViewChange();
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>((_scene_lu + _scene_rd) / 2);
	emit signalViewEvent(e_center);
}



void Kinstrument_view::grabTransInfo()
{

	_antiT.reset();
	_antiT.scale(1.0 / transform().m11(), 1.0 / transform().m22());

	_XYantiScale.setX(abs(_antiT.m11()));
	_XYantiScale.setY(abs(_antiT.m22()));

}


void Kinstrument_view::slotScaleChanged(qreal x_scale, qreal y_scale)
{
	scale(x_scale, y_scale);
	onViewChange();
	
}

void Kinstrument_view::slotLabelCenterChanged(qreal label_x, qreal label_y)
{
	if(!_scene)
		return;

	qreal x = _scene->label_w_to_x(label_x);
	qreal y = _scene->label_h_to_y(label_y);
	slotCenterChanged(x, y);
}

void Kinstrument_view::slotLabelMouseChanged(qreal label_x, qreal label_y)
{
	if(!_scene)
		return;

	qreal x = _scene->label_w_to_x(label_x);
	qreal y = _scene->label_h_to_y(label_y);
	slotMouseChanged(x, y);
}

void Kinstrument_view::slotCenterChanged(qreal scene_x, qreal scene_y)
{
	if (!_scene)
		return;

	centerOn(scene_x, scene_y);
	onViewChange();

	//onMouseChanged(scene_x, scene_y);
}

void Kinstrument_view::slotMouseChanged(qreal scene_x, qreal scene_y)
{
	if (!_scene)
		return;

	QPointF view_mouse_pos = mapFromScene(scene_x, scene_y);
	onMouseChange(view_mouse_pos);

	paintCrosshair();
}

void Kinstrument_view::slotViewEvent(std::shared_ptr<view_event> event)
{
	switch (event->type())
	{
	case view_event::type_t::on_transform_change:
	{
		const view_event_transform_change* e = (view_event_transform_change*)event.get();
		slotSetTransform(e->transform(), e->combine());
	}
	break;
	case view_event::type_t::on_scene_center_change:
	{
		const view_event_scene_center_change* e = (view_event_scene_center_change*)event.get();
		slotCenterChanged(e->scene_x(), e->scene_y());
	}
	break;
	case view_event::type_t::on_scene_mouse_change:
	{
		const view_event_scene_mouse_change* e = (view_event_scene_mouse_change*)event.get();
		slotMouseChanged(e->scene_x(), e->scene_y());
	}
	break;
	default:
		break;
	}
}

void Kinstrument_view::resetTransform()
{
	QGraphicsView::resetTransform();
	grabTransInfo();
}

void Kinstrument_view::scale(qreal x_scale, qreal y_scale)
{
	QGraphicsView::scale(x_scale, y_scale);
	grabTransInfo();
}


void Kinstrument_view::setTransform(const QTransform& matrix, bool combine)
{
	QGraphicsView::setTransform(matrix, combine);
	grabTransInfo();
}



void Kinstrument_view::zoomIn()
{
	//qDebug() << "ZoomIn()";

	if (_scene->items().isEmpty())
		return;

	setMouseTracking(false);
	QPointF scene_center = (_scene_lu + _scene_rd) / 2;
	QPointF pre_pos_dlt = scene_center - _scene_mouse;
	QPointF now_center = pre_pos_dlt / zoom_rate + _scene_mouse;// +QPointF(this->transform().m11() * zoom_fix.x(), this->transform().m22() * zoom_fix.y());
	//QPointF fix = QPointF(zoom_fix.x() / this->transform().m11(), zoom_fix.y() / this->transform().m22());	//unkown reson...
	//qDebug() << "+";
	//qDebug() << "_scene_mouse " << _scene_mouse;
	//qDebug() << "scene_center " << scene_center;
	//qDebug() << "pre_pos_dlt " << pre_pos_dlt;
	//qDebug() << "now_center " << now_center;
	//qDebug() << "pre_0 " << QGraphicsView::mapToScene(0, 0);

	QTransform T;
	T.scale(zoom_rate, zoom_rate);

	QTransform transform = this->transform();
	transform *= T;
	setTransform(transform);
	std::shared_ptr<view_event_transform_change> e_trans = std::make_shared<view_event_transform_change>(this->transform(), false);
	emit signalViewEvent(e_trans);

	centerOn(now_center);
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>(now_center.x(), now_center.y());
	emit signalViewEvent(e_center);
	//qDebug() << "now_0 " << QGraphicsView::mapToScene(0, 0);
	//qDebug() << "now_center " << QGraphicsView::mapToScene(width() / 2, height() / 2);
	//QPointF um = QGraphicsView::mapToScene(width() / 2, height() / 2) - now_center;
	//qDebug() << "unmatch = " << um;
	//qDebug() << "xs=" << this->transform().m11() << "  ys=" << this->transform().m22();
	//qDebug() << "unmatch * s = " << um.x() * this->transform().m11() << um.y() * this->transform().m22();
	//qDebug() << "---------------------------------";

	setMouseTracking(true);
	return;

}
void Kinstrument_view::zoomOut()
{
	// qDebug() << "ZoomOut()";

	if (_scene->items().isEmpty())
		return;

	setMouseTracking(false);
	QPointF scene_center = (_scene_lu + _scene_rd) / 2;
	QPointF pre_pos_dlt = scene_center - _scene_mouse;
	QPointF now_center = pre_pos_dlt * zoom_rate + _scene_mouse;
	//QPointF fix = QPointF(zoom_fix.x() / this->transform().m11(), zoom_fix.y() / this->transform().m22());	//unkown reson...
	//qDebug() << "-";
	//qDebug() << "_scene_mouse " << _scene_mouse;
	//qDebug() << "scene_center " << scene_center;
	//qDebug() << "pre_pos_dlt " << pre_pos_dlt;
	//qDebug() << "now_center " << now_center;
	//qDebug() << "pre_0 " << QGraphicsView::mapToScene(0, 0);

	QTransform T;
	T.scale(1.0 / zoom_rate, 1.0 / zoom_rate);

	QTransform transform = this->transform();
	transform *= T;
	setTransform(transform);
	std::shared_ptr<view_event_transform_change> e_trans = std::make_shared<view_event_transform_change>(this->transform(), false);
	emit signalViewEvent(e_trans);

	centerOn(now_center);
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>(now_center.x(), now_center.y());
	emit signalViewEvent(e_center);
	//qDebug() << "now_0 " << QGraphicsView::mapToScene(0, 0);
	//qDebug() << "now_center " << QGraphicsView::mapToScene(width() / 2, height() / 2);
	//QPointF um = QGraphicsView::mapToScene(width() / 2, height() / 2) - now_center;
	//qDebug() << "unmatch = " << um;
	//qDebug() << "xs=" << this->transform().m11() << "  ys=" << this->transform().m22();
	//qDebug() << "unmatch * s = " << um.x() * this->transform().m11() << um.y() * this->transform().m22();
	//qDebug() << "---------------------------------";

	setMouseTracking(true);
	return;

}

QPointF Kinstrument_view::getXYscale()
{
	QTransform T;// = antiTransformY();
	qreal xscale = transform().m11();
	qreal yscale = transform().m22();
	if (xscale < 0)xscale = -xscale;
	if (yscale < 0)yscale = -yscale;
	return std::move(QPointF(xscale, yscale));
}

void Kinstrument_view::paintLabel(QList<QGraphicsItem*>& pGroup, const QPointF& view_pos, const QString& txt, const color_pair_t& color_pair, int zV,
	bool onLeft, int shift, bool auto_fit)
{
	QPointF scene_pos = mapToScene(view_pos.x(), view_pos.y());
	//if (scene_pos.y() < 0)
	//	scene_pos.setY(0);
	//if (scene_pos.y() > _scene->height())
	//	scene_pos.setY(_scene->height());
	//if (scene_pos.x() < 0)
	//	scene_pos.setX(0);
	//if (scene_pos.x() > _scene->width())
	//	scene_pos.setX(_scene->width());

	qreal x = scene_pos.x();//_scene_pos.x();
	qreal y = scene_pos.y();

	qreal rx = getXYscale().x();//_antiT.m11();
	qreal ry = getXYscale().y();
	Klabel_t* label_x = new Klabel_t;
	label_x->setText(txt);

	if (onLeft) {
		x -= (shift + label_x->boundingRect().width()) / rx;
	}
	else {
		x += shift / rx;
	}
	if (auto_fit) {
		if (x < _scene_lu.x())
			x = _scene_lu.x();
		if (x >= _scene_rd.x() - (label_x->boundingRect().width() + (verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0)) / rx)
			x = _scene_rd.x() - (label_x->boundingRect().width() + (verticalScrollBar()->isVisible() ? verticalScrollBar()->width() : 0)) / rx;
		//y += 20 / ry;
		if (y >= _scene_rd.y() - (label_x->boundingRect().height() + (horizontalScrollBar()->isVisible() ? horizontalScrollBar()->height() : 0)) / ry)
			y -= (label_x->boundingRect().height() + (horizontalScrollBar()->isVisible() ? horizontalScrollBar()->height() : 0)) / ry;
	}

	label_x->setTransform(_antiT);

	label_x->setColor(color_pair);
	label_x->setPos(x, y);
	label_x->setZValue(zV);
	pGroup.append(label_x);

}

QGraphicsItemGroup* Kinstrument_view::paintCrosshairAt(const QPointF& scene_pos)
{
	QList<QGraphicsItem*> crossLine;

	QPen xPen = QPen(_colorpalette->crosshair, 1/*width*/, Qt::DashLine);
	QPen yPen = QPen(_colorpalette->crosshair, 1/*width*/, Qt::DashLine);
	xPen.setCosmetic(true);
	yPen.setCosmetic(true);

	QPointF view_pos = QGraphicsView::mapFromScene(scene_pos.x(), scene_pos.y());

	if (scene_pos.y() >= _scene->sceneRect().y() && scene_pos.y() < _scene->sceneRect().y()+_scene->height()) {
		QGraphicsLineItem* hline = new QGraphicsLineItem;
		hline->setLine(_scene_lu.x(), scene_pos.y(), _scene_rd.x(), scene_pos.y());
		hline->setPen(xPen);
		crossLine.append(hline);
	}

	if (scene_pos.x() >= _scene->sceneRect().x() && scene_pos.x() < _scene->sceneRect().x()+_scene->width()) {
		QGraphicsLineItem* vline = new QGraphicsLineItem;
		vline->setLine(scene_pos.x(), _scene_lu.y(), scene_pos.x(), _scene_rd.y());
		vline->setPen(yPen);
		crossLine.append(vline);
	}


	{
		QString txt_y = _scene->y_to_label_h(scene_pos.y());
		paintLabel(crossLine, view_pos, txt_y, _colorpalette->labels[0], 100);
	}

	{
		QString txt_x = _scene->x_to_label_w(scene_pos.x());
		paintLabel(crossLine, {view_pos.x(), double(height()-40)}, txt_x, _colorpalette->labels[0], 100, false, 0);
	}


	return _scene->createItemGroup(crossLine);
}


void Kinstrument_view::paintCrosshair()
{
	if(_crossLine){
		_scene->removeItem(_crossLine);
	}
	_crossLine = paintCrosshairAt(_scene_mouse);

}

void Kinstrument_view::paintGridLines()
{
	if (_gridLines) {
		_scene->removeItem(_gridLines);
	}
	QList<QGraphicsItem*> gridLines;

	QPen xPen = QPen(_colorpalette->grid.front, 1/*width*/, Qt::DashLine);
	QPen yPen = QPen(_colorpalette->grid.front, 1/*width*/, Qt::DashLine);
	xPen.setCosmetic(true);
	yPen.setCosmetic(true);


	for (qreal i = _ctx.sc_val_h_min; i < _ctx.sc_val_h_max* (1.01 + _grid_h_gap); i = _isLogCoor? i*(1.0 + _grid_h_gap) : i+_ctx.sc_val_h_max * _grid_h_gap) {
		qreal y = _scene->val_h_to_y(i);
		QGraphicsLineItem* line = new QGraphicsLineItem(_scene->sceneRect().x(), y, _scene->sceneRect().x() + _scene->sceneRect().width(), y);
		line->setPen(xPen);
		gridLines.append(line);

		//QString txt = _scene->y_to_val_label(y);
		//paintLabel(_gridLines, mapFromScene(_scene_lu.x(), y), txt, _colorpalette->labels[1], 99, false, 0);
	}

	for (int w = _ctx.sc_val_w_min; w <= _ctx.sc_val_w_max; w += _grid_w_gap) {
		qreal x = _scene->val_w_to_near_x(w);
		QGraphicsLineItem* line = new QGraphicsLineItem(x, _scene->sceneRect().y(), x, _scene->sceneRect().y() + _scene->sceneRect().height());
		line->setPen(yPen);
		gridLines.append(line);

		//QString txt = _scene->x_to_val_label(i);
		//paintLabel(_gridLines, mapFromScene(x, _scene_lu.y()), txt, _colorpalette->labels[1], 99, false, 0);
	}
	_gridLines = _scene->createItemGroup(gridLines);
}

void Kinstrument_view::paintGridLabels()
{
	if (_gridLabels) {
		_scene->removeItem(_gridLabels);
	}
	QList<QGraphicsItem*> gridLabels;

	for (qreal i = _ctx.sc_val_h_min; i < _ctx.sc_val_h_max * (1.01 + _grid_h_gap); i = _isLogCoor ? i * (1.0 + _grid_h_gap) : i + _ctx.sc_val_h_max * _grid_h_gap) {
		qreal y = _scene->val_h_to_y(i);
		if (y < _scene_lu.y() || y > _scene_rd.y())
			continue;
		QString txt = _scene->y_to_label_h(y);
		paintLabel(gridLabels, mapFromScene(_scene_lu.x(), y), txt, _colorpalette->labels[2], 99, false, 0, false);
	}

	for (int w = _ctx.sc_val_w_min; w <= _ctx.sc_val_w_max; w += _grid_w_gap) {
		qreal x = _scene->val_w_to_near_x(w);
		if (x < _scene_lu.x() || x > _scene_rd.x())
			continue;
		QString txt = _scene->x_to_label_w(x);
		paintLabel(gridLabels, mapFromScene(x, _scene_lu.y()), txt, _colorpalette->labels[2], 99, false, 0, false);
	}
	_gridLabels = _scene->createItemGroup(gridLabels);
}


void Kinstrument_view::setCtx_test()
{

	qreal v_m = 1;
	qreal v_M = 20;
	qreal w_m = 0;
	qreal w_M = 100;
	ctx_t test_ctx = {
		v_m,
		v_M,
		w_m,
		w_M
	};
	setCtx(test_ctx);
}


void Kinstrument_view::fitView()
{
	centerOn(_scene->val_w_to_x(_scene->getCtx().val_w_max()), _scene->val_h_to_y((_scene->getCtx().val_h_max() + _scene->getCtx().val_h_min()) / 2));
	onViewChange();
	std::shared_ptr<view_event_scene_center_change> e_center = std::make_shared<view_event_scene_center_change>((_scene_lu + _scene_rd) / 2);
	emit signalViewEvent(e_center);
	std::shared_ptr<view_event_transform_change> e_trans = std::make_shared<view_event_transform_change>(this->transform(), false);
	emit signalViewEvent(e_trans);
}




}
}
