#pragma once

#include "qt_Kviewer/s4KlogicItem.h"
#include "qt_Kviewer/s4Kinstrument_Kline_scene.h"

namespace S4{
namespace QT{
    
	
class KlogicTrade_t : public KlogicItemGroup_t
{
public:
    explicit KlogicTrade_t(Kinstrument_scene* scene):
        KlogicItemGroup_t(scene)
    {
    }

    ~KlogicTrade_t(){
    }

    inline void setLineWidth(int width) { _line_width = width; }

    inline void setValue(const std::vector<S4::s4_history_trade_t>& history)
    {
        _history = history;
    }

    inline void setColor(const color_box_t& color_positive, const color_box_t& color_negtive)
    {
        _color_positive = color_positive;
        _color_negtive = color_negtive;
    }

    virtual void mkGroupItems() override;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) Q_DECL_OVERRIDE
    {
        if (event->button() == Qt::LeftButton) {
            setSelected(true);
        }
        QGraphicsItemGroup::mousePressEvent(event);
    }
private:
    //Kinstrument_Kline_scene* _scene;

    std::vector<S4::s4_history_trade_t> _history;
    color_box_t _color_positive;
    color_box_t _color_negtive;
	int _line_width = 2;

	std::set<size_t> _aborted_open;
	std::set<size_t> _closed_open;
    bool _found_open;
    bool _found_close;
private:
	void paint_send_open(size_t trade_i);
	void paint_abort_open(size_t trade_i);
	void paint_open(size_t trade_i);
	void paint_send_close(size_t trade_i);
	void paint_abort_close(size_t trade_i);
	void paint_close(size_t trade_i, trade_opt_t type);

    void paint_ts(const s4_history_trade_t& trade);

    void paint_oc_link(qreal open_val_w, qreal deal_open, qreal close_val_w, qreal deal_close);
};



} // namespace QT
} // namespace S4
