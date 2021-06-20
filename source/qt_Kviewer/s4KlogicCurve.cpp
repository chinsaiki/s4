#include "qt_Kviewer/s4KlogicCurve.h"

namespace S4 {
namespace QT {


void KlogicCurve_t::mkGroupItems(void){
    if(!_scene) return;

    _x_max = 0;
    _x_min = 1e9;
    _y_max = 0;
    _y_min = 1e9;

    QList<QPointF> paintList;
    qreal x;
    qreal y;
    for(const auto& p : _valueList){
        x = _scene->val_w_to_x(p.x());
        y = _scene->val_h_to_y(p.y());
        if (x > _x_max) _x_max = x;
        if (x < _x_min) _x_min = x;
        if (y > _y_max) _y_max = y;
        if (y < _y_min) _y_min = y;

        paintList.push_back({x, y});
    }

    QPen pen(_line_color, _line_width, _style, Qt::RoundCap, Qt::MiterJoin);
    pen.setCosmetic(true);

    qreal preX = paintList[0].x() - _x_min;
    qreal preY = paintList[0].y() - _y_min;
    QGraphicsEllipseItem* point = new QGraphicsEllipseItem(preX - _dot_size/2, preY - _dot_size/2,
                                                            _dot_size, _dot_size);
    point->setPen(QPen(Qt::NoPen));
    point->setBrush(_dot_color);
    point->setZValue(1);
    addToGroup(point);
    for (int i = 1; i < paintList.size(); ++i) {
        x = paintList[i].x() - _x_min;
        y = paintList[i].y() - _y_min;

        QGraphicsLineItem* pLine = new QGraphicsLineItem(preX, preY, x, y);
        pLine->setPen(pen);
        pLine->setZValue(0);
        addToGroup(pLine);

        //painter->drawPoint(QPointF(X, Y));
        //painter->drawEllipse(X, Y, 2, 2);
        
        preX = x;
        preY = y;
        point = new QGraphicsEllipseItem(preX - _line_width - _dot_size/2, preY - _line_width - _dot_size/2, 
            _line_width * 2 + _dot_size, _line_width * 2 + _dot_size);
        point->setPen(QPen(Qt::NoPen));
        point->setBrush(_dot_color);
        point->setZValue(1);
        addToGroup(point);
    }
    setPos({_x_min, _y_min});
}


}//QT
}//S4