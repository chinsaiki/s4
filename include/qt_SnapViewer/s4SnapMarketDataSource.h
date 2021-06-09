#pragma once

#include "types/s4type.h"
#include "qt_SnapViewer/s4SnapMarketDataSourceCfg.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QList>




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

    QList<snapMarketDataSourceCfg::cfg_t> getCfgs(void);

    
public slots:
	void onStart();
    void onAdd();
	void onSourceNameChange(QWidget*);
signals:
    void signal_start();
    void signal_stop();
    void signal_add();

private:
    Ui::snapMarketDataSource *ui;
	bool _started;
};


} // namespace QT
} // namespace S4
