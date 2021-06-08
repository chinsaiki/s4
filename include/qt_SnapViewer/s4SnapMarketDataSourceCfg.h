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
class snapMarketDataSourceCfg;
}
QT_END_NAMESPACE


namespace S4{
namespace QT{
    
//tabs, = [market]+[basic]

class snapMarketDataSourceCfg: public QWidget
{
    Q_OBJECT
public:
    snapMarketDataSourceCfg(QWidget *parent = nullptr);
    virtual ~snapMarketDataSourceCfg();

    
public slots:

    
private:
    Ui::snapMarketDataSourceCfg *ui;

};


} // namespace QT
} // namespace S4
