#include "qt_Kviewer/s4Kinstrument_indicator_scene.h"
#include "common/s4time.h"
#include "qt_Kviewer/s4KlogicBar.h"
#include "qt_Kviewer/s4KlogicCurve.h"
#include "qt_Kviewer/s4KlogicRect.h"


namespace S4{
namespace QT{

#define BAR_Z (51)
#define MA_Z (52)
    
Kinstrument_indicator_scene::Kinstrument_indicator_scene(QWidget* parent):
    Kinstrument_scene(parent)
{
}

//datetime_t or time_t -> date_seq
qreal Kinstrument_indicator_scene::label_w_to_val_w(uint64_t l) const 
{
    if (!_label_map_w.size())
        return 0;

    if (_label_map_w.count(l))
        return _label_map_w.at(l);

    if (l<_label_map_w.begin()->first)
        return _label_map_w.begin()->second;

    if (l>_label_map_w.end()->first)
        return _label_map_w.end()->second;

    for (auto& m : _label_map_w){
        if (m.first > l)
            return m.second;
    }
    return _label_map_w.end()->second;
}

QString Kinstrument_indicator_scene::x_to_label_w(qreal x) const 
{
    int val_w = int(x_to_val_w(x) + 0.5);
    QString txt;
    if (_w_map_label.count(val_w)){
        if (_indCtx.timeMode == timeMode_t::tDAY){
            txt.sprintf("%s", date_to_str(utc_to_date(_w_map_label.at(val_w))).c_str());
        }else{
            txt.sprintf("%s", utc_to_str(_w_map_label.at(val_w)).c_str());
        }
    }else{
        txt.sprintf("N/A");
    }
    return txt;
}

QString Kinstrument_indicator_scene::y_to_label_h(qreal y) const
{
    qreal val_h = y_to_val_h(y);
    QString txt;
    txt.sprintf("%0.2f", val_h / 100.0);    //TODO: tdx stock price precision = 1% of Yuan 精度=分
    return txt;
}

//TODO: save for _view to make label
void Kinstrument_indicator_scene::paint(indCtx_t indCtx, std::shared_ptr<data_panel_t> data_panel)
{
    clear();
    _indCtx = indCtx; 
    _data_panel = data_panel;

    if (indCtx.type == IND_VOL || indCtx.type == IND_AMT){
        paint_volumn();
    }
    return;
}

std::shared_ptr<infKQ_t> Kinstrument_indicator_scene::check_data_volumn(void) const
{
    std::shared_ptr<infKQ_t> pInfoKQ;
    if (_indCtx.timeMode == timeMode_t::tDAY){
        pInfoKQ = _data_panel->info.pDayKQ;
    }else if (_indCtx.timeMode == timeMode_t::tMINU){
        pInfoKQ = _data_panel->info.pMinuKQ;
    }else{  //TODO
        return pInfoKQ;
    }

    if (!pInfoKQ || !pInfoKQ->size())
        return nullptr;
    if (!pInfoKQ->isNewAtBack())
        return nullptr;

    return pInfoKQ;
}

void Kinstrument_indicator_scene::calcCtx_volumn()
{
    std::shared_ptr<infKQ_t> pInfoKQ = check_data_volumn();
    if (!pInfoKQ)
        return;

    ctx_t ctx;
    if (_indCtx.type == IND_VOL){
        ctx.set_val_h_max((*pInfoKQ)[0]->volume);
        ctx.set_val_h_min(0);
    } else if (_indCtx.type == IND_AMT){
        ctx.set_val_h_max((*pInfoKQ)[0]->amount);
        ctx.set_val_h_min(0);
    }
    ctx.set_val_w_max(0);
    ctx.set_val_w_min(0);

    int n = 0;
    _label_map_w.clear();
    _w_map_label.clear();
    for(const auto& d : *pInfoKQ)
    {
        if (_indCtx.type == IND_VOL){
            if (d->volume > ctx.val_h_max())    ctx.set_val_h_max(d->volume);
        } else if (_indCtx.type == IND_AMT){
            if (d->amount > ctx.val_h_max())    ctx.set_val_h_max(d->amount);
        }
        ctx.set_val_w_max(n);
        _label_map_w[d->_time] = n;
        _w_map_label[n] = d->_time;
        n++;
    }

	

    setCtx(ctx);

	initSceneCanvas();
}

bool Kinstrument_indicator_scene::get_valPos(int w_seq, QPointF& val) const
{
	std::shared_ptr<infKQ_t> _pInfoKQ = check_data_volumn();

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

    if (_indCtx.type == IND_VOL){
        val.setY((*_pInfoKQ)[nb]->volume);
    } else if (_indCtx.type == IND_AMT){
        val.setY((*_pInfoKQ)[nb]->amount);
    }

    return true;
}

void Kinstrument_indicator_scene::paint_volumn()
{
    std::shared_ptr<infKQ_t> pInfoKQ = check_data_volumn();
    if (!pInfoKQ)
        return;

    calcCtx_volumn();

    QList<logicRectData_h_t> vols;

    for(size_t i=0; i < pInfoKQ->size(); ++i)
    {
        const auto& d = (*pInfoKQ)[i];
        logicRectData_h_t v;
        v.seq = label_w_to_val_w(d->_time);
        if (_indCtx.type == IND_VOL){
            v.val_top = d->volume;
        } else if (_indCtx.type == IND_AMT){
            v.val_top = d->amount;
        }
        v.val_btm = 0;

        if (!vols.size()){
            v.positive = (*pInfoKQ)[i]->close_fq() > (*pInfoKQ)[i]->open_fq();
        }else{
            v.positive = (*pInfoKQ)[i]->close_fq() > (*pInfoKQ)[i-1]->close_fq();
        }
        vols.push_back(std::move(v));
    }

	KlogicRectGroup_h_t* Rect = new KlogicRectGroup_h_t(this);
	Rect->setColor(_colorpalette->positive_boxes[1], _colorpalette->negtive_boxes[1]);
	Rect->setValue(vols);
	Rect->mkGroupItems();
	Rect->setZValue(BAR_Z);
	this->addItem(Rect);

}

} // namespace QT
} // namespace S4


