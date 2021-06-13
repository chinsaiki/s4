#pragma once

#include "qt_common/s4qt_colorpalette.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <memory>

namespace S4 {
namespace QT {

//指标和K线画布的基类
class Kinstrument_scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Kinstrument_scene(QWidget* parent = 0);

    virtual ~Kinstrument_scene(){}
    class ctx_t {
    public:
        ctx_t(){}
        ctx_t(
            qreal val_h_min,
            qreal val_h_max,
            qreal val_w_min,
            qreal val_w_max,
            qreal val_h_10percent_pxl = 20,
            qreal val_w_pxl = 16
        ) :
            _val_h_min(val_h_min),
            _val_h_max(val_h_max),
            _val_w_min(val_w_min),
            _val_w_max(val_w_max),
            _val_h_10percent_pxl(val_h_10percent_pxl),
            _val_w_pxl(val_w_pxl)
        {
        }

        ctx_t(const ctx_t& i):
            _val_h_min(i.val_h_min()),
            _val_h_max(i.val_h_max()),
            _val_w_min(i.val_w_min()),
            _val_w_max(i.val_w_max()),
            _val_h_10percent_pxl(i.val_h_10percent_pxl()),
            _val_w_pxl(i.val_w_pxl())
        {}

        qreal val_h_min() const { return _val_h_min; }    // h_min is at bottom, but y=0 is at top
        qreal val_h_max() const { return _val_h_max; }
        qreal val_w_min() const { return _val_w_min; }    // w_min at x=0
        qreal val_w_max() const { return _val_w_max; }
        void set_val_h_min(qreal v) { _val_h_min = v; }    // h_min is at bottom, but y=0 is at top
        void set_val_h_max(qreal v) { _val_h_max = v; }
        void set_val_w_min(qreal v) { _val_w_min = v; }    // w_min at x=0
		void set_val_w_max(qreal v) { _val_w_max = v; }

		qreal val_h_min_margin() const { return _val_h_min_margin; }
		qreal val_h_max_margin() const { return _val_h_max_margin; }
		qreal val_w_min_margin() const { return _val_w_min_margin; }
		qreal val_w_max_margin() const { return _val_w_max_margin; }
		void set_val_h_min_margin(qreal v) { _val_h_min_margin = v; }
		void set_val_h_max_margin(qreal v) { _val_h_max_margin = v; }
		void set_val_w_min_margin(qreal v) { _val_w_min_margin = v; }
		void set_val_w_max_margin(qreal v) { _val_w_max_margin = v; }

        qreal val_h_10percent_pxl() const { return _val_h_10percent_pxl; }    //
        qreal val_w_pxl() const { return _val_w_pxl; }
        
    private:
        qreal _val_h_min = -1;    // h_min is at bottom, but y=0 is at top
        qreal _val_h_max = -1;
        qreal _val_w_min = -1;    // w_min at x=0
        qreal _val_w_max = -1;

		qreal _val_h_max_margin = 0.1;	// 10%*_val_h_max
		qreal _val_h_min_margin = 0.1;	// 10%*_val_h_min
		qreal _val_w_max_margin = 15;	// *w_pxl
		qreal _val_w_min_margin = 5;	// *w_pxl

        qreal _val_h_10percent_pxl = 120; //10% of (val_h_max - val_h_min) map to pixel
        qreal _val_w_pxl = 16;           //1 of val_w  map to pixel
    };

    void setCtx(const ctx_t& ctx) {
        _ctx = ctx;
    }

    inline const ctx_t& getCtx(void) const {
        return _ctx;
    }

    inline void setLogCoor(bool log)
    {
        _isLogCoor = log;
    }

	//Only for log coordinate, must be the same as view, 0.1 = 10%, 0.2 = 20%
    inline void setGridGap_h(qreal gap_h) {
        _grid_h_gap = gap_h;
    }

    //h & w is logic value, as price & date_seq, origin at left-bottom
    //y & x is in scene-coordinate, origin at left-top
    virtual qreal val_h_to_y(qreal h) const;
    virtual qreal val_w_to_x(qreal w) const;
    virtual qreal y_to_val_h(qreal y) const;
    virtual qreal x_to_val_w(qreal x) const;

    //default: label = logic value
    //for K-bar: label is datetime_t or time_t, logic value is date_sequence, x/y is scene-coordinate.
    virtual qreal label_w_to_x(uint64_t l) const{
        return val_w_to_x(label_w_to_val_w(l));
    };

    //datetime_t or time_t -> date_seq
    virtual qreal label_w_to_val_w(uint64_t l) const{
        return l;
    };

    //percent -> price
    virtual qreal label_h_to_val_h(qreal l) const{
        return l;
    };
    //percent -> scene.coordinate
    virtual qreal label_h_to_y(qreal l) const{
        return val_h_to_y(label_h_to_val_h(l));
    };

    //for label-mark
    virtual QString y_to_label_h(qreal y) const;
    virtual QString x_to_label_w(qreal x) const;

    virtual bool get_valPos(int w_seq, QPointF& val) const
    {
        qreal val_w = w_seq;
        if (val_w < _ctx.val_w_min())
            val_w = _ctx.val_w_min();
        if (val_w > _ctx.val_w_max())
            val_w = _ctx.val_w_max();
        val.setX(val_w);
        val.setY((_ctx.val_h_max() + _ctx.val_h_min())/2);
        return true;
    }
//signals:
//    void cursorPosition(QPointF);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE;
    
protected:
    void initSceneCanvas();

    void drawBK();

    void drawTest();
    void drawTest_bar();
    void drawTest_curve();
protected:
    QPointF _pos;
    ctx_t _ctx;
    bool _isLogCoor = true;
    qreal _h_val_pxl = 1;
    qreal _h_val_pxl_base = 1;
    qreal _h_log_pxl = 1;
    qreal _h_log_max = 1;
    qreal _h_log_min = 1;
    qreal _w_val_pxl = 1;
    qreal _grid_h_gap = 0.1;  //10%

    std::shared_ptr<qt_colorpalette_t> _colorpalette;
};

}
}
