#pragma once

#include "qt_Kviewer/s4Kinstrument_scene.h"
#include "qt_common/s4qt_data_if.h"


namespace S4{
namespace QT{

class Kinstrument_indicator_scene : public Kinstrument_scene
{
public:
    //TODO: move to data_if
    enum ind_type{
		IND_VOL = 0,
		IND_AMT,
        IND_MACD,
        //
        IND_SIZE
    };

    struct indCtx_t{
        ind_type type;
        timeMode_t timeMode;
        //TODO: indicator parameters

        QString toString(void){
            QString ret;
            switch (type)
            {
            case IND_VOL:       ret += "VOLUMN";
				break;
			case IND_AMT:       ret += "AMOUNT";
				break;
            case IND_MACD:      ret += "MACD";
                break;
            default:            ret += "N/A";
                break;
            }

            return ret;
        }
    };
public:
    Kinstrument_indicator_scene(QWidget* parent = 0);
    virtual ~Kinstrument_indicator_scene(){}

    //datetime_t or time_t -> date_seq
    virtual qreal label_w_to_val_w(uint64_t l) const override;

    virtual QString x_to_label_w(qreal x) const override;

    virtual QString y_to_label_h(qreal y) const override;
    
    virtual bool get_valPos(int w_seq, QPointF& val) const override;
    
public slots:
    void paint(indCtx_t indCtx, std::shared_ptr<data_panel_t> data_panel);

protected:
    std::map<uint64_t, int> _label_map_w;
    std::map<int, uint64_t> _w_map_label;

    indCtx_t _indCtx;
    std::shared_ptr<data_panel_t> _data_panel;
protected:

    virtual std::shared_ptr<infKQ_t> check_data_volumn(void) const;
    void calcCtx_volumn(void);
    void paint_volumn(void);

};

} // namespace QT
} // namespace S4


