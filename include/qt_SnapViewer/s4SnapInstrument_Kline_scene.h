#pragma once

#include "qt_Kviewer/s4Kinstrument_scene.h"
#include "qt_common/s4qt_data_if.h"
#include "data/s4infSnap.h"


namespace S4{
namespace QT{

class snapInstrument_Kline_scene : public Kinstrument_scene
{
public:
	struct KCtx_t {
		timeMode_t timeMode;
	};
public:
    snapInstrument_Kline_scene(QWidget* parent = 0);
    virtual ~snapInstrument_Kline_scene(){}

    //void setInfoKQ(const std::shared_ptr<infKQ_t>& pInfoKQ);

    //void setMAmap(const std::shared_ptr<std::map<int, std::shared_ptr<maQ_t>>>& pMAmap);

    void paint(const KCtx_t&, std::shared_ptr<data_panel_t>);

    void paint(const infSnapQ_ptr& pSnaps);

    virtual qreal val_w_to_near_x(qreal w) const override;
    //datetime_t or time_t -> date_seq
    virtual qreal label_w_to_val_w(uint64_t l) const override;

    virtual QString x_to_label_w(qreal x) const override;

    virtual QString y_to_label_h(qreal y) const override;

    //w_seq: -1 = latest valid val_w
    virtual bool get_valPos(int w_seq, QPointF& val) const override;

	virtual qreal label_w_to_best_val_h(uint64_t l) const;

    virtual QPointF get_label_near(const QPointF& scene_pos, QPointF& scene_label_pos) const;
private:
    std::map<uint64_t, int> _label_map_w;
    std::map<int, uint64_t> _w_map_label;

    //std::shared_ptr<infKQ_t> _pInfoKQ;
	//std::shared_ptr<std::map<int, std::shared_ptr<maQ_t>>> _pMAmap;
	KCtx_t _KCtx;
	std::shared_ptr<data_panel_t> _data_panel;
private:
	std::shared_ptr<infKQ_t> check_data(void) const;

    void calcCtx(void);

    void paint_infoKQ(void);
    
    void paint_MAmap(void);
	void paint_MA(int scope, const std::shared_ptr<maQ_t>& maQ);

	void paint_trade(void);

    void calcCtx(const infSnapQ_ptr& pSnaps);
	void paint_Snap_price(const infSnapQ_ptr& pSnaps);

private:

};

} // namespace QT
} // namespace S4


