#pragma once

#include "types/s4type.h"
#include "qt_common/sharedCharArray_ptr.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QTableView>
#include <QLabel>


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
    
public slots:
	void onL2Data_instrument_snap(const S4::sharedCharArray_ptr&);
	void onL2Data_index_snap(const S4::sharedCharArray_ptr&);
	void onL2Data_order(const S4::sharedCharArray_ptr&);
	void onL2Data_exec(const S4::sharedCharArray_ptr&);
signals:
	void signal_L2Data_instrument_snap(const S4::sharedCharArray_ptr&);
	void signal_L2Data_index_snap(const S4::sharedCharArray_ptr&);
	void signal_L2Data_order(const S4::sharedCharArray_ptr&);
	void signal_L2Data_exec(const S4::sharedCharArray_ptr&);
    
private:
    // 
    QTableView* _level_tv;

    //
    QTableView* _info_tv;

    //
    QTableView* _order_tv;
    
    //
    QTableView* _exec_tv;

    QLabel* _order_info;
    QLabel* _exec_info;

private:

    long long _order_cnt;
    long long _exec_cnt;

};


} // namespace QT
} // namespace S4
