#pragma once

#include "qt_Kviewer/s4KlogicItem.h"


namespace S4{
namespace QT{

class KlogicCurve_t : public KlogicItem_t
{
public:
    explicit KlogicCurve_t(Kinstrument_scene *scene):
        KlogicItem_t(scene)
    {
    }

    // explicit KlogicCurve_t(QGraphicsItem *parent = nullptr):
    //     QGraphicsItemGroup(parent),
    //     _scene(nullptr)
    // {
    // }


    ~KlogicCurve_t(){
    }

    inline void setLineStyle(enum Qt::PenStyle style) { _style = style; };
    inline void setLineWidth(int width) { _line_width = width; }
    inline void setDotSize(qreal size) { _dot_size = size; }


    //point.x: logic value in w; point.y: logic value in h
    inline void setValue(const QList<QPointF>& pointList, bool replace = true)
    {
        if (replace){
            _valueList = pointList;
        }else{
            _valueList += pointList;
        }
    }

    //hlist: logic value in h
    void setValue(const QList<qreal>& hlist, bool replace = true) 
    {
        if (replace){
            _valueList.clear();
        }

        int base = _valueList.size();
        for (auto& val : hlist){
            _valueList.push_back(std::move(QPointF(base++, val)));
        }
        // for (int i = 0; i < hlist.size(); ++i) {
        //     if (hlist[i] > 0) {
        //         _valueList.push_back(std::move(QPoint(base + i, hlist[i])));
        //     }
        // }
    }

    inline void setColor(const color_pair_t& color_pair)
    {
        _line_color = color_pair.back;
        _dot_color = color_pair.front;
    }

    virtual void mkGroupItems();
private:
    QList<QPointF> _valueList;
    QColor _line_color;
    QColor _dot_color;
    enum Qt::PenStyle _style;
    int _line_width = 1;

    qreal _x_max, _x_min, _y_max, _y_min;   //in scene
    qreal _dot_size = 1.0;
private:
    void removeFromGroup(QGraphicsItem* item) {};
    
    // void checkPointSeq(void){
    //     for( int i=0; i<_valueList.size()-2; ++i){
    //         if (_valueList[i].x() < _valueList[i+1].x()){
    //            TODO?
    //         }
    //     }
    // }

};



    
} // namespace QT
} // namespace S4