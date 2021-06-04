#pragma once

#include "types/s4type.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QTableView>


namespace S4{
namespace QT{
    
//view of tables = [level] + [info] + [order] + [exec]
class snapInstrument_tableMarket: public QWidget
{
    Q_OBJECT
public:
    snapInstrument_tableMarket(int snapLeves_nb,QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent* )
    {
        //qDebug() << "snapInstrument_tableMarket " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
    }

public slots:

    void addSnaps(const std::vector<tdx_snap_t>&);
    // void addOrders(const std::vector<ssz_sbe_order_t>&);
    // void addExecs(const std::vector<ssz_sbe_exec_t>&);
    
private:
    // 
    QTableView* _level_tv;

    //
    QTableView* _info_tv;

    //
    QTableView* _order_tv;
    
    //
    QTableView* _exec_tv;

private:

};


} // namespace QT
} // namespace S4
