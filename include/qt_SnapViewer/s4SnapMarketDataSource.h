#pragma once

#include "types/s4type.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>




QT_BEGIN_NAMESPACE
namespace Ui {
class snapMarketDataSource;
}
QT_END_NAMESPACE


namespace S4{
namespace QT{
    
//tabs, = [market]+[basic]

class snapMarketDataSource: public QWidget
{
    Q_OBJECT
public:
    snapMarketDataSource(QWidget *parent = nullptr);
    virtual ~snapMarketDataSource();

    
public slots:
    void onStart(){}
    void onStop(){}
    void onAdd();
signals:
    void signal_start();
    void signal_stop();
    void signal_add();

private:
    Ui::snapMarketDataSource *ui;

};


} // namespace QT
} // namespace S4
