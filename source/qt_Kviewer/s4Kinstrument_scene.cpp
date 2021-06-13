#include "qt_Kviewer/s4Kinstrument_scene.h"
#include "qt_Kviewer/s4KlogicCurve.h"
#include "qt_Kviewer/s4KlogicBar.h"
#include "qt_Kviewer/s4KlogicRect.h"
#include "qt_Kviewer/s4KlogicTriangle.h"

#include <QGraphicsLineItem>
#include <QtCore/qmath.h>
#include <QDebug>

namespace S4 {
namespace QT {

#define SCENE_Z 50

Kinstrument_scene::Kinstrument_scene(QWidget* parent):
    QGraphicsScene(parent)
{
    _colorpalette = std::make_shared<qt_colorpalette_t>();
    drawBK();
	// drawTest();
}

void Kinstrument_scene::initSceneCanvas()
{
	//expand ctx
	_ctx.set_val_h_max(_ctx.val_h_max()*(1.0 + _ctx.val_h_max_margin()));
	_ctx.set_val_h_min(_ctx.val_h_min()*(1.0 - _ctx.val_h_min_margin()));
	_ctx.set_val_w_max(_ctx.val_w_max() + _ctx.val_w_max_margin());
	_ctx.set_val_w_min(_ctx.val_w_min() - _ctx.val_w_min_margin());

	qreal height;
	if (_isLogCoor) {	
		height = _ctx.val_h_10percent_pxl() * qLn(_ctx.val_h_max() / _ctx.val_h_min()) / qLn(1.0 + _grid_h_gap);
	}
	else {
		height = (_ctx.val_h_max() - _ctx.val_h_min());
		if (height > 960) {
			height = 960;
		}
		else if (height < 100) {
			height = 100;
		}
	}
	qreal width = _ctx.val_w_max() - _ctx.val_w_min();
	width *= _ctx.val_w_pxl();
	
	setSceneRect(-_ctx.val_w_pxl() * 0.5, 0, width, height);

	_h_val_pxl = (_ctx.val_h_max() - _ctx.val_h_min()) / (this->height());

	_h_log_max = qLn(_ctx.val_h_max()) / qLn(1.0 + _grid_h_gap);
	_h_log_min = qLn(_ctx.val_h_min()) / qLn(1.0 + _grid_h_gap);
	_h_log_pxl = (_h_log_max - _h_log_min) / (this->height());

	_w_val_pxl = (_ctx.val_w_max() - _ctx.val_w_min()) / (this->width() - 1);
}

//Kinstrument_scene::void cursorPosition(QPointF);
qreal Kinstrument_scene::val_h_to_y(qreal val) const
{
	qreal y_o;
	if (!_isLogCoor) {
		y_o = height() - (val - _ctx.val_h_min()) / _h_val_pxl;	//
	}
	else {
		y_o = qLn(_ctx.val_h_max() / val) / qLn(1.0 + _grid_h_gap) / _h_log_pxl;
	}
	return y_o;
}

qreal Kinstrument_scene::y_to_val_h(qreal y) const
{
	if (!_isLogCoor) {
		return _ctx.val_h_max() - y * _h_val_pxl;
	}
	else
	{
		qreal log_coor = _h_log_max - y * _h_log_pxl;
		qreal ex = qExp(log_coor * qLn(1.0 + _grid_h_gap));
		return ex;
	}
}

qreal Kinstrument_scene::val_w_to_x(qreal val) const
{
	qreal x_o;
	x_o = (val - _ctx.val_w_min()) / _w_val_pxl + sceneRect().x();	//
	return x_o;
}

qreal Kinstrument_scene::x_to_val_w(qreal x) const
{
	return (x - sceneRect().x()) * _w_val_pxl + _ctx.val_w_min();
}
QString Kinstrument_scene::y_to_label_h(qreal y) const
{
	qreal val_h = y_to_val_h(y);
	QString txt;
	txt.sprintf("%0.2f", val_h);
	return txt;
}
QString Kinstrument_scene::x_to_label_w(qreal x) const
{
	int val_w = int(x_to_val_w(x) + 0.5);
	QString txt;
	txt.sprintf("%d", val_w);
	return txt;
}

void Kinstrument_scene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mousePressEvent(event);
	//if (event->button() == Qt::LeftButton) {
	//	qDebug() << "scene selected";
	//}
}
void Kinstrument_scene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsScene::mouseReleaseEvent(event);
}

void Kinstrument_scene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{

}

void Kinstrument_scene::drawBK()
{
	//this->setAutoFillBackground(true);
	//QPalette palette;
	//palette.setColor(QPalette::Background, _colorpalette->background);
	//this->setPalette(palette);
	//this->drawBackground(, this->sceneRect())
	QBrush brush(_colorpalette->background, Qt::SolidPattern);
	setBackgroundBrush(brush);
}

void Kinstrument_scene::drawTest()
{
	qreal v_m = 1;
	qreal v_M = 20;
	qreal w_m = 0;
	qreal w_M = 100;
	ctx_t test_ctx(
		v_m,
		v_M,
		w_m,
		w_M
	);
	setCtx(test_ctx);

	QPen pen = QPen(_colorpalette->positive_boxes[0].body, 1.0/*this param Sets the pen width to the given width in pixels with integer precision.*/);

	qreal x_w_min = val_w_to_x(w_m);
	qreal x_w_max = val_w_to_x(w_M);
	qreal y_v_min = height();//val_h_to_y(v_m);
	qreal y_v_max = 0;//val_h_to_y(v_M);
	//V
	for (qreal v = v_m; v < v_M*(1.0 + _grid_h_gap); v *=(1.0 + _grid_h_gap)) {
		qreal y = val_h_to_y(v);
		QGraphicsLineItem* line = new QGraphicsLineItem(x_w_min, y, x_w_max, y);
		line->setPen(pen);
		line->setZValue(SCENE_Z);
		addItem(line);

		QString txt;
		txt.sprintf("%0.2f", v);
		QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem;
		text->setText(txt);
		text->setPos(x_w_min, y);
		text->setBrush(_colorpalette->labels[1].front);
		text->setZValue(SCENE_Z);
		addItem(text);

		QGraphicsRectItem* box = new QGraphicsRectItem;
		box->setRect(text->boundingRect());
		box->setPen(QPen(Qt::NoPen));
		box->setPos(x_w_min, y);
		box->setBrush(_colorpalette->labels[1].back);
		text->setZValue(SCENE_Z-1);
		addItem(box);
	}

	pen = QPen(_colorpalette->negtive_boxes[0].body, 1.0/*this param Sets the pen width to the given width in pixels with integer precision.*/);
	//H
	for (qreal w = w_m; w < w_M + 1; w += 1) {
		qreal x = val_w_to_x(w);
		QGraphicsLineItem* line = new QGraphicsLineItem(x, y_v_max, x, y_v_min);
		line->setPen(pen);
		line->setZValue(SCENE_Z);
		addItem(line);

		QString txt;
		txt.sprintf("%0.0f", w);
		QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem;
		text->setText(txt);
		text->setPos(x, y_v_max);
		text->setBrush(_colorpalette->labels[1].front);
		text->setZValue(SCENE_Z);
		addItem(text);

		QGraphicsRectItem* box = new QGraphicsRectItem;
		box->setRect(text->boundingRect());
		box->setPen(QPen(Qt::NoPen));
		box->setPos(x, y_v_max);
		box->setBrush(_colorpalette->labels[1].back);
		text->setZValue(SCENE_Z-1);
		addItem(box);
	}
	//for (qreal x = 0; x < x_max; x += 100) {
	//	for (qreal y = 0; y < y_max; y += 100) {
	//		QString txt;
	//		txt.sprintf("%0.0f,%0.0f", x, y);
	//		QGraphicsSimpleTextItem* text = new QGraphicsSimpleTextItem;
	//		text->setText(txt);
	//		text->setPos(x, y);
	//		text->setBrush(_colorpalette->labels[1].front);
	//		text->setZValue(100);
	//		addItem(text);

	//		QGraphicsRectItem* box = new QGraphicsRectItem;
	//		box->setRect(text->boundingRect());
	//		box->setPen(QPen(Qt::NoPen));
	//		box->setPos(x, y);
	//		box->setBrush(_colorpalette->labels[1].back);
	//		text->setZValue(99);
	//		addItem(box);
	//	}
	//}
	drawTest_curve();
	drawTest_bar();

	qDebug() << "---------";
	qDebug() << x_w_min << x_w_max;
	qDebug() << y_v_min << y_v_max;
	qDebug() << sceneRect();
	qDebug() << v_m << sceneRect().y() + sceneRect().height() << val_h_to_y(v_m) << y_to_val_h(val_h_to_y(v_m));
	qDebug() << v_M << sceneRect().y() << val_h_to_y(v_M) << y_to_val_h(val_h_to_y(v_M));
	qDebug() << w_m << sceneRect().x() << val_w_to_x(w_m) << x_to_val_w(val_w_to_x(w_m));
	qDebug() << w_M << sceneRect().x() + sceneRect().width() << val_w_to_x(w_M) << x_to_val_w(val_w_to_x(w_M));
}

void Kinstrument_scene::drawTest_curve()
{
	QList<QPointF> dot = {
		{0, 2},
		{10, 2},
		{20, 3},
		{30, 8},
		{40, 12},
		{50, 15},
		{60, 20},
	};
	KlogicCurve_t* curve = new KlogicCurve_t(this);
	curve->setLineStyle(Qt::PenStyle::DashLine);
	curve->setColor(_colorpalette->curve[0]);
	curve->setLineWidth(2);
	curve->setValue(dot);
	curve->mkGroupItems();
	curve->setZValue(SCENE_Z + 2);
	this->addItem(curve);

}

void Kinstrument_scene::drawTest_bar()
{
	QList<logicBarData_t> dayBar = {
		{10, 12, 13, 14, 12, 12},
		{11, 13, 12, 15, 11, 13}
	};
	KlogicBarGroup_t* bar = new KlogicBarGroup_t(this);
	bar->setColor(_colorpalette->positive_boxes[0], _colorpalette->negtive_boxes[0]);
	bar->setType(KlogicBar_t::barType_t::BAR_JPN);
	bar->setLineWidth(2);
	bar->setValue(dayBar);
	bar->mkGroupItems();
	bar->setZValue(SCENE_Z + 1);
	this->addItem(bar);

	QList<logicRectData_h_t> dayVol = {
		{10, 6, _ctx.val_h_min(), true},
		{11, 9.2, _ctx.val_h_min(), false},
	};
	KlogicRectGroup_h_t* Rect = new KlogicRectGroup_h_t(this);
	Rect->setColor(_colorpalette->positive_boxes[1], _colorpalette->negtive_boxes[1]);
	Rect->setValue(dayVol);
	Rect->mkGroupItems();
	Rect->setZValue(SCENE_Z + 1);
	this->addItem(Rect);


	QList<logicRectData_w_t> dayCyc = {
		{12, 1, 60, _ctx.val_w_max(), true},
		{11, 1, 80, _ctx.val_w_max(), false},
	};
	KlogicRectGroup_w_t* RectW = new KlogicRectGroup_w_t(this);
	RectW->setColor(_colorpalette->positive_boxes[2], _colorpalette->negtive_boxes[2]);
	RectW->setValue(dayCyc);
	RectW->mkGroupItems();
	RectW->setZValue(SCENE_Z + 1);
	this->addItem(RectW);

	KlogicTriangle_t* bi = new KlogicTriangle_t(this);
	bi->setDirect(KlogicTriangle_t::dirMode_t::Tri_RT);
	bi->setValue(10, 12.5);
	bi->setColor(_colorpalette->positive_boxes[1]);
	bi->mkGroupItems();
	bi->setZValue(SCENE_Z + 2);
	this->addItem(bi);


	KlogicTriangle_t* bo = new KlogicTriangle_t(this);
	bo->setDirect(KlogicTriangle_t::dirMode_t::Tri_LF);
	bo->setValue(11, 14);
	bo->setColor(_colorpalette->negtive_boxes[1]);
	bo->mkGroupItems();
	bo->setZValue(SCENE_Z + 2);
	this->addItem(bo);
}
}//QT
}//S4
