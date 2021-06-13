#pragma once

#include "types/s4type.h"
#include "network/sockutil.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>




QT_BEGIN_NAMESPACE
namespace Ui {
class marketDataSourceCfg;
}
QT_END_NAMESPACE


namespace S4{
namespace QT{
    

class marketDataSourceCfg: public QWidget
{
    Q_OBJECT
public:
    marketDataSourceCfg(const QString& Label, QWidget *parent = nullptr);
    virtual ~marketDataSourceCfg();

    struct cfg_t{
        std::string Name;
        std::string listen_IP;
        uint16_t Port;
        std::string SourceType;
        std::string local_IP;
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
    Ui::marketDataSourceCfg *ui;

    bool _editable = true;
    QList<SockUtil::nic_description_t> _NICs_info;
    
};


} // namespace QT
} // namespace S4
