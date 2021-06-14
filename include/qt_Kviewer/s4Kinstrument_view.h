#pragma once

#include "qt_common/s4view_event.h"
#include "qt_common/s4qt_colorpalette.h"
#include "qt_Kviewer/s4Kinstrument_scene.h"

#include <QWidget>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QGraphicsItemGroup>

namespace S4 {
namespace QT {

//指标和K线视图的基类
class Kinstrument_view : public QGraphicsView
{
    Q_OBJECT
public:
    Kinstrument_view(Kinstrument_scene*scene, QWidget *parent = 0);

    virtual ~Kinstrument_view(){}
//signals:
//    void cursorPosition(QPointF);

    inline void setLogCoor(bool log)    //scene需要同步坐标类型
    {
        _isLogCoor = log;
    }

    //only for grid line&label
    struct ctx_t
    {
        qreal sc_val_h_min = -1;    // h_min is at bottum, but y=0 is at top
        qreal sc_val_h_max = -1;
        qreal sc_val_w_min = -1;    // w_min at x=0
        qreal sc_val_w_max = -1;

        qreal val_h_min() const { return sc_val_h_min; }    // h_min is at bottum, but y=0 is at top
        qreal val_h_max() const { return sc_val_h_max; }
        qreal val_w_min() const { return sc_val_w_min; }    // w_min at x=0
        qreal val_w_max() const { return sc_val_w_max; }
        void set_val_h_min(qreal v) { sc_val_h_min = v; }    // h_min is at bottum, but y=0 is at top
        void set_val_h_max(qreal v) { sc_val_h_max = v; }
        void set_val_w_min(qreal v) { sc_val_w_min = v; }    // w_min at x=0
        void set_val_w_max(qreal v) { sc_val_w_max = v; }
    };

    virtual void setCtx(const ctx_t& ctx)
    {
        _ctx = ctx;
        // paintGridLines();   //TODO:need a new interface for painting at first.
    }
        
    virtual void paintGridLines();

    inline void setGridGap_h(qreal gap_h) {
        _grid_h_gap = gap_h;
	}

	inline void setGridGap_w(qreal gap_w) {
		_grid_w_gap = gap_w;
	}

    virtual void fitView();

signals:
	//void signalScaleChanged(qreal x_scale, qreal y_scale);
	//void signalSetTransform(const QTransform&, bool);
	//void signalLabelCenterChanged(qreal label_x, qreal label_y);
	//void signalLabelMouseChanged(qreal label_x, qreal label_y);
	//void signalCenterChanged(qreal scene_x, qreal scene_y);
	//void signalMouseChanged(qreal scene_x, qreal scene_y);

	void signalViewEvent(std::shared_ptr<view_event>);

public slots:
    void verticalScrollvalueChanged(int v = 0);
    void horizontalScrollvalueChanged(int v = 0);

    virtual void slotScaleChanged(qreal x_scale, qreal y_scale);
	virtual void slotSetTransform(const QTransform&, bool) { grabTransInfo(); };
    virtual void slotLabelCenterChanged(qreal label_x, qreal label_y);
	virtual void slotLabelMouseChanged(qreal label_x, qreal label_y);
	virtual void slotCenterChanged(qreal scene_x, qreal scene_y);
	virtual void slotMouseChanged(qreal scene_x, qreal scene_y);

	virtual void slotViewEvent(std::shared_ptr<view_event>);


	bool isPaint() const { return _isPaint; }
	void setIsPaint(bool v) { _isPaint = v; }
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent* event) override;
    virtual void dragMoveEvent(QDragMoveEvent* event) override;
    
    //bool viewportEvent(QEvent *e) override;
    //virtual void paintEvent(QPaintEvent*) override;

    void scale(qreal x_scale, qreal y_scale);
    void setTransform(const QTransform& matrix, bool combine = false);
    void resetTransform();

protected:
    Kinstrument_scene* _scene;
    std::shared_ptr<qt_colorpalette_t> _colorpalette;
    
    QGraphicsItem* _mouse_item;

    QPointF _scene_mouse;
    QPointF _scene_lu;
    QPointF _scene_rd;

    QPointF _view_mouse_pos;

    bool _drag_to_move = false;
    QPointF _mouse_press_bgn_center;
    QPointF _mouse_press_bgn_pos;
    QPointF _mouse_press_end_pos;

    QTransform _antiT;
    QPointF _XYantiScale;

    bool _isLogCoor = true;
    qreal _grid_h_gap = 0.1;  //10%
    qreal _grid_w_gap = 20;
    ctx_t _ctx;
protected:
    qreal val_to_sceneh(qreal val);
    qreal sceneh_to_val(qreal val);

    void setCtx_test();

    virtual void onViewChange(void);
    virtual void onMouseChange(qreal ax, qreal ay);
    virtual void onMouseChange(const QPointF& view_mouse);

    virtual void zoomIn();
    virtual void zoomOut();

    virtual void grabTransInfo();
    virtual QPointF getXYscale();

    QGraphicsItemGroup* _crossLine = nullptr;
    virtual void paintCrosshair();
    QGraphicsItemGroup* paintCrosshairAt(const QPointF& scene_pos);

    QGraphicsItemGroup* _gridLines = nullptr;

    QGraphicsItemGroup* _gridLabels = nullptr;
    virtual void paintGridLabels();

    //paint map from view-position to scen-position
    virtual void paintLabel(QList<QGraphicsItem*>& pGroup, const QPointF& view_pos, const QString& txt, const color_pair_t& color_pair, int zV,
        bool onLeft = true, int shift = 20, bool auto_fit = true);

private:
    bool _isPaint = false;
};

}
}
