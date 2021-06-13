#include "qt_SnapViewer/s4SnapInstrument_Kline_scene.h"
#include "common/s4time.h"
#include "qt_Kviewer/s4KlogicBar.h"
#include "qt_Kviewer/s4KlogicCurve.h"
#include "qt_Kviewer/s4KlogicTrade.h"

namespace S4{
namespace QT{

#define BAR_Z (51)
#define MA_Z (52)
#define TRADE_Z (60)
    
snapInstrument_Kline_scene::snapInstrument_Kline_scene(QWidget* parent):
    Kinstrument_scene(parent)
{
}

//void snapInstrument_Kline_scene::setInfoKQ(const std::shared_ptr<infKQ_t>& pInfoKQ){
//    _pInfoKQ = pInfoKQ;
//}
//
//void snapInstrument_Kline_scene::setMAmap(const std::shared_ptr<std::map<int, std::shared_ptr<maQ_t>>>& pMAmap){
//    _pMAmap = pMAmap;
//}


void snapInstrument_Kline_scene::paint(const KCtx_t& ctx, std::shared_ptr<data_panel_t> data_panel)
{
	_KCtx = ctx;
	_data_panel = data_panel;

    calcCtx();
    initSceneCanvas();
    paint_infoKQ();
    paint_MAmap();
	paint_trade();
}


//datetime_t or time_t -> date_seq
qreal snapInstrument_Kline_scene::label_w_to_val_w(uint64_t l) const 
{
    uint64_t _l = l;
    if (_KCtx.timeMode == timeMode_t::tDAY) {
        _l = date_to_utc(utc_to_date(l));
    }
    else if (_KCtx.timeMode == timeMode_t::tMINU) {
        time_date_t date;
        time_minuSec_t minu;
        date = utc_to_date(l, &minu);
        minu = minu / 100;
        minu = minu * 100;
        _l = date_to_utc(date, minu);
    }
    else {
        throw TimerError("unsupport timeMode: not tDAY nor tMINU");
    }

    if (!_label_map_w.size())
        return 0;

    if (_label_map_w.count(_l))
        return _label_map_w.at(_l);

    if (_l <_label_map_w.begin()->first)
        return _label_map_w.begin()->second;

    if (_l >_label_map_w.rbegin()->first)
        return _label_map_w.rbegin()->second;

    for (auto& m : _label_map_w){
        if (m.first > _l)
            return m.second;
    }
    return _label_map_w.end()->second;
}

qreal snapInstrument_Kline_scene::label_w_to_best_val_h(uint64_t l) const
{
	const std::shared_ptr<infKQ_t> _pInfoKQ = check_data();
	if (!_pInfoKQ)
		return (_ctx.val_h_max() + _ctx.val_h_min()) / 2;

    return _pInfoKQ->getLastInfo(l)->close_fq();
}

QString snapInstrument_Kline_scene::x_to_label_w(qreal x) const 
{
    int val_w = int(x_to_val_w(x) + 0.5);
    QString txt;
    if (_w_map_label.count(val_w)){
        if (_KCtx.timeMode == timeMode_t::tDAY){
            txt.sprintf("%s", date_to_str(utc_to_date(_w_map_label.at(val_w))).c_str());
        }else{
            txt.sprintf("%s", utc_to_str(_w_map_label.at(val_w)).c_str());
        }
    }else{
		if (_label_map_w.size() && _label_map_w.rbegin()->second + 1 == val_w) {
			txt.sprintf("Dummy Next");
        }
		else {
			txt.sprintf("N/A");
        }
    }
    return txt;
}

QString snapInstrument_Kline_scene::y_to_label_h(qreal y) const
{
    qreal val_h = y_to_val_h(y);
    QString txt;
    txt.sprintf("%0.2f", val_h / 100.0);    //TODO: tdx stock price precision = 1% of Yuan 精度=分
    return txt;
}


std::shared_ptr<infKQ_t> snapInstrument_Kline_scene::check_data(void) const
{
    if (!_data_panel){
        return nullptr;
    }

	std::shared_ptr<infKQ_t> pInfoKQ;
	if (_KCtx.timeMode == timeMode_t::tDAY) {
		pInfoKQ = _data_panel->info.pDayKQ;
	}
	else if (_KCtx.timeMode == timeMode_t::tMINU) {
		pInfoKQ = _data_panel->info.pMinuKQ;
	}
	else {  //TODO
		return pInfoKQ;
	}

	if (!pInfoKQ || !pInfoKQ->size())
		return nullptr;
	if (!pInfoKQ->isNewAtBack())
		return nullptr;

	return pInfoKQ;
}

void snapInstrument_Kline_scene::calcCtx(void)
{
	std::shared_ptr<infKQ_t> _pInfoKQ = check_data();
    if (!_pInfoKQ)
        return;

    ctx_t ctx;
    ctx.set_val_h_max((*_pInfoKQ)[0]->high_fq());
    ctx.set_val_h_min((*_pInfoKQ)[0]->low_fq());
    ctx.set_val_w_max(0);
	ctx.set_val_w_min(0);

    int n = 0;
    _label_map_w.clear();
    _w_map_label.clear();
    for(const auto& d : *_pInfoKQ)
    {
        if (d->high > ctx.val_h_max()){
            ctx.set_val_h_max(d->high_fq());
        }
        if (d->low < ctx.val_h_min()){
            ctx.set_val_h_min(d->low_fq());
        }
        ctx.set_val_w_max(n);
        _label_map_w[d->_time] = n;
        _w_map_label[n] = d->_time;
        n++;
    }

    setCtx(ctx);
}

//TODO: save for _view to make label
void snapInstrument_Kline_scene::paint_infoKQ(void)
{
	std::shared_ptr<infKQ_t> _pInfoKQ = check_data();

    if (!_pInfoKQ)
        return;

    QList<logicBarData_t> bars;
    
    for(const auto& d : *_pInfoKQ)
    {
        logicBarData_t bar;
        bar.seq = label_w_to_val_w(d->_time);
        bar.O = d->open_fq();
        bar.C = d->close_fq();
        bar.H = d->high_fq();
        bar.L = d->low_fq();
        bar.Avg = d->prcAvg_fq();

        if (!bars.size()){
            bar.lastC = bar.O;
        }else{
            bar.lastC = bars.back().C;
        }
        bars.push_back(std::move(bar));
    }
    
    KlogicBarGroup_t* barGroup = new KlogicBarGroup_t(this);
    barGroup->setColor(_colorpalette->positive_boxes[0], _colorpalette->negtive_boxes[0]);
    barGroup->setType(KlogicBar_t::barType_t::BAR_JPN);
    barGroup->setLineWidth(4);
    barGroup->setValue(bars);
    barGroup->mkGroupItems();
    barGroup->setZValue(BAR_Z);
	this->addItem(barGroup);

	/* paint next day */
	logicBarData_t nextDay;
	nextDay.seq = _label_map_w.rbegin()->second + 1;
    
	//up
    nextDay.lastC = nextDay.L = nextDay.O = (*_pInfoKQ->rbegin())->close_fq();
    nextDay.H = nextDay.C = UP_10p((*_pInfoKQ->rbegin())->close_fq());
	KlogicBar_t* barU = new KlogicBar_t(this);
	barU->setColor(_colorpalette->positive_boxes[2], _colorpalette->negtive_boxes[2]);
	barU->setType(KlogicBar_t::barType_t::BAR_USA);
	barU->setLineWidth(0);
	barU->setAlpha(100);
	barU->setValue(nextDay);
	barU->mkGroupItems();
    this->addItem(barU);

	//dn
	nextDay.H = nextDay.O = (*_pInfoKQ->rbegin())->close_fq();
	nextDay.L = nextDay.C = DN_10p((*_pInfoKQ->rbegin())->close_fq());
	KlogicBar_t* barD = new KlogicBar_t(this);
	barD->setColor(_colorpalette->positive_boxes[2], _colorpalette->negtive_boxes[2]);
	barD->setType(KlogicBar_t::barType_t::BAR_USA);
	barD->setLineWidth(0);
	barD->setAlpha(100);
	barD->setValue(nextDay);
	barD->mkGroupItems();
	this->addItem(barD);

}

void snapInstrument_Kline_scene::paint_MAmap(void){
    int n = 0;
    for(auto& m : *_data_panel->info.pMAlib){
        paint_MA(n++, m.second);
    }
}

//TODO: save ind&scope&value&colors for _view to make label
void snapInstrument_Kline_scene::paint_MA(int ind, const std::shared_ptr<maQ_t>& maQ)
{
    if( !maQ || !maQ->size())
        return;

    QList<QPointF> dots;
    for (auto& ma : *maQ){
        QPointF dot;
        dot.setX(label_w_to_val_w(ma->_time));
        dot.setY(ma->C());
        dots.push_back(std::move(dot));
    }
	KlogicCurve_t* curve = new KlogicCurve_t(this);
	curve->setLineStyle(Qt::PenStyle::SolidLine);
	curve->setColor(_colorpalette->curve[ind % _colorpalette->curve.size()]);
	curve->setLineWidth(1);
	curve->setValue(dots);
	curve->mkGroupItems();
	curve->setZValue(MA_Z);
	this->addItem(curve);
}

void snapInstrument_Kline_scene::paint_trade()
{
    KlogicTrade_t* trade_group = new KlogicTrade_t(this);
	trade_group->setValue(_data_panel->history);
    trade_group->setColor(_colorpalette->positive_boxes[1], _colorpalette->negtive_boxes[1]);
    trade_group->mkGroupItems();
    trade_group->setZValue(TRADE_Z);
	this->addItem(trade_group);

}

bool snapInstrument_Kline_scene::get_trade_valPos(int seq, QPointF& val) const
{
    if (!_data_panel->history.size())
        return false;

    size_t nb;
    if (seq >= 0) {
        nb = seq % _data_panel->history.size();
    }
    else {
        nb = (-seq) % _data_panel->history.size();
        if (nb != 0)
            nb = _data_panel->history.size() - nb;

    }

    val.setX(label_w_to_val_w(_data_panel->history[nb].time_utcSec));
    val.setY(label_w_to_best_val_h(_data_panel->history[nb].time_utcSec));
    
    return true;
}

bool snapInstrument_Kline_scene::get_valPos(int w_seq, QPointF& val) const
{
	std::shared_ptr<infKQ_t> _pInfoKQ = check_data();

    if (!_pInfoKQ)
        return false;

    size_t nb;
    if (w_seq >= 0) {
        nb = w_seq % _pInfoKQ->size();
    }
    else {
        nb = (-w_seq) % _pInfoKQ->size();
        if (nb != 0)
            nb = _pInfoKQ->size() - nb;
    }
    
    val.setX(label_w_to_val_w((*_pInfoKQ)[nb]->_time));
    val.setY((*_pInfoKQ)[nb]->open_fq());

    return true;
}

void snapInstrument_Kline_scene::paint(const infSnapQ_ptr& pSnaps)
{
    _KCtx.timeMode = timeMode_t::tSnap;

    calcCtx(pSnaps);
    initSceneCanvas();
}

void snapInstrument_Kline_scene::calcCtx(const infSnapQ_ptr& pSnaps)
{
    if (!pSnaps)
        return;

    ctx_t ctx;
    // ctx.set_val_h_max(pSnaps->front()->high);
    // ctx.set_val_h_min(pSnaps->front()->low);
    ctx.set_val_h_max(pSnaps->front()->up20P());
    ctx.set_val_h_min(pSnaps->front()->dn20P());
    ctx.set_val_w_max(0);
	ctx.set_val_w_min(0);

    //time_t bgn_time = pSnaps->back()->_time;

    int n = 0;
    _label_map_w.clear();
    _w_map_label.clear();
    for(const auto& d : *pSnaps)
    {
        // if (d->high > ctx.val_h_max()){
        //     ctx.set_val_h_max(d->high);
        // }
        // if (d->low < ctx.val_h_min()){
        //     ctx.set_val_h_min(d->low);
        // }
        _label_map_w[d->_time] = n; //TODO
        _w_map_label[n] = d->_time;
        n++;
    }

    if (pSnaps->front()->_MinmuSec >= 150000){
        ctx.set_val_w_max(n);
    }else{
        ctx.set_val_w_max(STK_SNAP_NUM_MAX);
    }

    setCtx(ctx);
}

} // namespace QT
} // namespace S4


