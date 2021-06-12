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
    

class snapMarketDataSourceCfg: public QWidget
{
    Q_OBJECT
public:
    snapMarketDataSourceCfg(const QString& Label, QWidget *parent = nullptr);
    virtual ~snapMarketDataSourceCfg();

    struct cfg_t{
        std::string Name;
        std::string IP;
        uint16_t Port;
        std::string SourceType;
    };

    struct cfg_t getCfg() const;

	QString label() const;
public slots:
    void slot_onButtom();
	void slot_disableEdit();
	void slot_enableEdit();
signals:
    void signal_fixed(QWidget* );
private:
    Ui::snapMarketDataSourceCfg *ui;

    bool _editable = true;
    
};


} // namespace QT
} // namespace S4
