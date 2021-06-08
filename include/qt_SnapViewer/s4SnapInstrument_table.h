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
    
//tabs, = [market]+[basic]

class snapInstrument_table: public QTabWidget
{
    Q_OBJECT
public:
    snapInstrument_table(int snapLeves_nb, QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent* )
    {
        //qDebug() << "snapInstrument_table " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
    }
    
public slots:

    void addSnaps(const std::vector<tdx_snap_t>&);
    // void addOrders(const std::vector<ssz_sbe_order_t>&);
    // void addExecs(const std::vector<ssz_sbe_exec_t>&);

    //TODO
    void addBasic(){}
    
public slots:
	void onL2Data_instrument_snap(const std::string&);
	void onL2Data_index_snap(const std::string&);
	void onL2Data_order(const std::string&);
	void onL2Data_exec(const std::string&);
signals:
	void signal_L2Data_instrument_snap(const std::string&);
	void signal_L2Data_index_snap(const std::string&);
	void signal_L2Data_order(const std::string&);
	void signal_L2Data_exec(const std::string&);
    
private:
    // std::map<int, Kinstrument_indicator_scene::indCtx_t> _tab_ctx;

    QWidget* _market;
    QWidget* _basic;

};


} // namespace QT
} // namespace S4
