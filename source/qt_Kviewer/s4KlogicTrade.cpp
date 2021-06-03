#include "qt_Kviewer/s4KlogicTrade.h"
#include "qt_Kviewer/s4KlogicTriangle.h"
#include "qt_Kviewer/s4KlogicCross.h"
#include "qt_Kviewer/s4KlogicLine.h"
#include "qt_Kviewer/s4Klabel.h"
#include "qt_Kviewer/s4KlogicRect.h"
#include "qt_Kviewer/s4Kinstrument_scene.h"
#include "trade/s4_history_trade.h"
#include "common/s4logger.h"

CREATE_LOCAL_LOGGER(logicTrade)

namespace S4{
namespace QT{

#define TRADE_Z (60)

void KlogicTrade_t::mkGroupItems(void)
{
    if(!_scene || !_history.size()) return;

    sort_history_trade(_history);

	_aborted_open.clear();
	_closed_open.clear();

	//qreal val_w_min = 1e9;
	//qreal val_w_max = 0;
	//qreal val_h_min = 1e9;
	//qreal val_h_max = 0;

	_found_close = false;
	_found_open = false;

	for (size_t i = 0; i < _history.size(); ++i) {

		//if (val_w > val_w_max) val_w_max = val_w;
		//if (val_w < val_w_min) val_w_min = val_w;
		//if (trade.order_open != -1 && trade.order_open > val_h_max) val_h_max = trade.order_open;
		//if (trade.order_open != -1 && trade.order_open < val_h_min) val_h_min = trade.order_open;
		//if (trade.deal_open != -1 && trade.deal_open > val_h_max) val_h_max = trade.deal_open;
		//if (trade.deal_open != -1 && trade.deal_open < val_h_min) val_h_min = trade.deal_open;
		//if (trade.order_close != -1 && trade.order_close > val_h_max) val_h_max = trade.order_close;
		//if (trade.order_close != -1 && trade.order_close < val_h_min) val_h_min = trade.order_close;
		//if (trade.deal_close != -1 && trade.deal_close > val_h_max) val_h_max = trade.deal_close;
		//if (trade.deal_close != -1 && trade.deal_close < val_h_min) val_h_min = trade.deal_close;


        switch (_history[i].optType)
        {
        case trade_opt_t::oSEND_OPEN:
			paint_send_open(i);
			break;
		case trade_opt_t::oABORT_OPEN:
			paint_abort_open(i);
			break;
		case trade_opt_t::oOPEN:
			paint_open(i);
			break;
		case trade_opt_t::oSEND_CLOSE:
			paint_send_close(i);
			break;
		case trade_opt_t::oABORT_CLOSE:
			paint_abort_close(i);
			break;
		case trade_opt_t::oTAKE:	/**/
		case trade_opt_t::oSTOP:
		case trade_opt_t::oCLOSE:
			paint_close(i, _history[i].optType);
			break;
        default:
			LCL_ERR("cannot paint trade_opt = {:} ", trade_opt_t_toString(_history[i].optType));
            break;
        }
		paint_ts(_history[i]);
    }

	//if not close, paint until end
	if (_found_open && !_found_close) {
		for (size_t j = _history.size()-1; j > 0; --j) {
			const auto& trade_j = _history[j];
			if (trade_j.optType == trade_opt_t::oOPEN && _closed_open.count(j) == 0) {
				paint_oc_link(_scene->label_w_to_val_w(trade_j.time_utcSec), trade_j.deal_open, _scene->getCtx().val_w_max() - _scene->getCtx().val_w_max_margin(), trade_j.deal_open);
			}
		}
	}

}

//TODO: paint through whole life of trade
void KlogicTrade_t::paint_ts(const s4_history_trade_t& trade)
{
	qreal val_w = _scene->label_w_to_val_w(trade.time_utcSec);
	if (trade.order_take != -1) {
		KlogicLine_t* l = new KlogicLine_t(_scene);
		l->setLineWidth(_line_width + 2);
		l->setValue(val_w, trade.order_take, val_w, trade.order_take);
		l->setColor(_color_positive.skin);
		l->setLineStyle(Qt::SolidLine);
		l->mkGroupItems();
		l->setZValue(TRADE_Z + 5);
		addToGroup(l);
	}

	if (trade.order_stop != -1) {
		KlogicLine_t* l = new KlogicLine_t(_scene);
		l->setLineWidth(_line_width + 2);
		l->setValue(val_w, trade.order_stop, val_w, trade.order_stop);
		l->setColor(_color_positive.skin);
		l->setLineStyle(Qt::SolidLine);
		l->mkGroupItems();
		l->setZValue(TRADE_Z + 5);
		addToGroup(l);
	}
}

void KlogicTrade_t::paint_send_open(size_t i)
{
	const auto& trade = _history[i];
	qreal val_w = _scene->label_w_to_val_w(trade.time_utcSec);

	KlogicTriangle_t* bi = new KlogicTriangle_t(_scene);
	bi->setDirect(KlogicTriangle_t::dirMode_t::Tri_RT);
	//bi->setAlpha(50);
	bi->setLineWidth(0);
	bi->setValue(val_w, trade.order_open);
	bi->setColor(_color_positive);
	bi->mkGroupItems();
	bi->setZValue(TRADE_Z);
	addToGroup(bi);

	
}

void KlogicTrade_t::paint_open(size_t i)
{
	const auto& trade = _history[i];
	qreal val_w = _scene->label_w_to_val_w(trade.time_utcSec);

	KlogicTriangle_t* bi = new KlogicTriangle_t(_scene);
	bi->setDirect(KlogicTriangle_t::dirMode_t::Tri_RT);
	bi->setAlpha(250);
	bi->setLineWidth(2);
	bi->setValue(val_w, trade.deal_open);
	bi->setColor(_color_positive);
	bi->mkGroupItems();
	bi->setZValue(TRADE_Z+3);
	addToGroup(bi);

	

	if (_found_open) {	//dual open
		//TODO
	}

	_found_open = true;
	_found_close = false;
}

void KlogicTrade_t::paint_abort_open(size_t i)
{
	const auto& trade = _history[i];
	qreal val_w = _scene->label_w_to_val_w(trade.time_utcSec);

	KlogicCross_t* cr = new KlogicCross_t(_scene);
	cr->setLineWidth(_line_width);
	cr->setValue(val_w, trade.order_open);
	cr->setColor(_color_positive.skin);
	cr->mkGroupItems();
	cr->setZValue(TRADE_Z + 1);
	addToGroup(cr);

	

	//not useful for TDX-style-orders, as they are not alive to next day.
	if (i > 0) {
		for (size_t j = i - 1; j > 0; --j) {
			const auto& trade_j = _history[j];
			if (trade_j.optType == trade_opt_t::oSEND_OPEN && _aborted_open.count(j) == 0) {
				KlogicLine_t* l = new KlogicLine_t(_scene);
				l->setLineWidth(_line_width);
				l->setValue(val_w, trade.order_open, _scene->label_w_to_val_w(trade_j.time_utcSec), trade_j.order_open);
				l->setColor(_color_positive.skin);
				l->setLineStyle(Qt::DotLine);
				l->setAutoExp(false);
				l->mkGroupItems();
				l->setZValue(TRADE_Z + 2);
				addToGroup(l);
				_aborted_open.insert(j);
			}
		}
	}
}


void KlogicTrade_t::paint_send_close(size_t i)
{
	const auto& trade = _history[i];
	qreal val_w = _scene->label_w_to_val_w(trade.time_utcSec);

	KlogicTriangle_t* bi = new KlogicTriangle_t(_scene);
	bi->setDirect(KlogicTriangle_t::dirMode_t::Tri_LF);
	//bi->setAlpha(50);
	bi->setLineWidth(0);
	bi->setValue(val_w, trade.order_close);
	bi->setColor(_color_negtive);
	bi->mkGroupItems();
	bi->setZValue(TRADE_Z);
	addToGroup(bi);

	
}

void KlogicTrade_t::paint_abort_close(size_t i)
{
	const auto& trade = _history[i];
	qreal val_w = _scene->label_w_to_val_w(trade.time_utcSec);

	KlogicCross_t* cr = new KlogicCross_t(_scene);
	cr->setLineWidth(_line_width);
	cr->setValue(val_w + 0.25, trade.order_close);
	cr->setColor(_color_positive.skin);
	cr->mkGroupItems();
	cr->setZValue(TRADE_Z + 1);
	addToGroup(cr);

	
}


void KlogicTrade_t::paint_close(size_t i, trade_opt_t type)
{
	const auto& trade = _history[i];
	qreal val_w = _scene->label_w_to_val_w(trade.time_utcSec);

	KlogicTriangle_t* bi = new KlogicTriangle_t(_scene);

	if (type == trade_opt_t::oCLOSE) {
		bi->setDirect(KlogicTriangle_t::dirMode_t::Tri_LF);
		bi->setColor(_color_negtive);
	}
	else if (type == trade_opt_t::oTAKE) {
		bi->setDirect(KlogicTriangle_t::dirMode_t::Tri_DN);
		bi->setColor(_color_positive);
	}
	else {	//oSTOP
		bi->setDirect(KlogicTriangle_t::dirMode_t::Tri_UP);
		bi->setColor(_color_negtive);
	}
	
	bi->setAlpha(250);
	bi->setLineWidth(2);
	bi->setValue(val_w, trade.deal_close);
	bi->mkGroupItems();
	bi->setZValue(TRADE_Z+3);
	addToGroup(bi);

	
	if (i > 0 && _found_open) {
		for (size_t j = i - 1; j > 0; --j) {
			const auto& trade_j = _history[j];
			if (trade_j.optType == trade_opt_t::oOPEN && _closed_open.count(j) == 0) {
				paint_oc_link(_scene->label_w_to_val_w(trade_j.time_utcSec), trade_j.deal_open, val_w, trade.deal_close);
				_closed_open.insert(j);
			}
		}
	}
	else {// no open
		paint_oc_link(val_w-20, trade.deal_close, val_w, trade.deal_close);	//
		Klabel_t* label = new Klabel_t();
		label->setText("miss open");
		label->setColor({_color_positive.body, _color_positive.skin});
		addToGroup(label);
	}
	_found_open = false;
	_found_close = true;
}

void KlogicTrade_t::paint_oc_link(qreal open_val_w, qreal deal_open, qreal close_val_w, qreal deal_close)
{
	KlogicLine_t* l = new KlogicLine_t(_scene);
	l->setLineWidth(_line_width);
	l->setValue(close_val_w, deal_close, open_val_w, deal_open);
	l->setColor(_color_positive.skin);
	l->setLineStyle(Qt::DotLine);
	l->setAutoExp(false);
	l->mkGroupItems();
	l->setZValue(TRADE_Z + 2);
	addToGroup(l);

	//paint label
	logicRectData_w_t rec_scop;
	rec_scop.val_h = (_scene->getCtx().val_h_max() + _scene->getCtx().val_h_min()) / 2;//val_h_max;
	rec_scop.val_h_scope = _scene->getCtx().val_h_max() - _scene->getCtx().val_h_min();
	rec_scop.val_lf = open_val_w;
	rec_scop.val_rt = close_val_w;
	rec_scop.positive = deal_open <= deal_close;
	KlogicRect_w_t* rect = new KlogicRect_w_t(_scene);
	rect->setValue(rec_scop);
	rect->setColor(_color_positive, _color_negtive);
	rect->setAlpha(50);
	rect->setZValue(1);
	rect->mkGroupItems();
	addToGroup(rect);

}


} // namespace QT
} // namespace S4