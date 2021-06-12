#pragma once

#include "types/s4type.h"
#include "qt_market/s4MarketDataSourceCfg.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QList>




QT_BEGIN_NAMESPACE
namespace Ui {
class marketDataSource;
}
QT_END_NAMESPACE


namespace S4{
namespace QT{
    
//tabs, = [market]+[basic]

class marketDataSource: public QWidget
{
    Q_OBJECT
public:
    marketDataSource(QWidget *parent = nullptr);
    virtual ~marketDataSource();

    QList<marketDataSourceCfg::cfg_t> getCfgs(void);

    
public slots:
	void onStart();
    void onAdd();
	void onSourceNameChange(QWidget*);
signals:
    void signal_start();
    void signal_stop();
    void signal_add();

private:
    Ui::marketDataSource *ui;
	bool _started;
};


} // namespace QT
} // namespace S4
