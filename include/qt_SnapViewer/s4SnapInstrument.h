#pragma once

#include "types/s4type.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>


namespace S4{
namespace QT{
    
//view in instrument-tab, = [chart] + [table]
class snapInstrument: public QWidget
{
    Q_OBJECT
public:
    snapInstrument(int snapLevels_nb, QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent* )
    {
        //qDebug() << "snapInstrument " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
    }

    
public slots:

    void addSnaps(const std::vector<tdx_snap_t>&);
    // void addOrders(const std::vector<ssz_sbe_order_t>&);
    // void addExecs(const std::vector<ssz_sbe_exec_t>&);

    //TODO
    void addBasic(){}
    
private:
    // K line tab
    // SnapInstrument_Kline_tab* _K_tab;

    // vol/indicator tab
    // SnapInstrument_indicator_tab* _indicator_tab;

    // cyc/... tab
    QTabWidget* _table;
    

private:

};


} // namespace QT
} // namespace S4
