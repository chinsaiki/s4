#pragma once

#include "network/L2_udp_recver_th.h"
#include "qt_common/dynamicQObject_mt.h"
#include <QThread>


namespace S4{
namespace QT{

class marketDataAgent : public QThread
{
    Q_OBJECT
public:
    marketDataAgent(std::shared_ptr<NW::L2DataQ_t>& pL2DataQ, std::shared_ptr<NW::L2CmdQ_t>& pCmdQ, QObject *parent = 0);

    void stop()
    {
        m_stop = true;
    }


    bool emitDynamicSignal(const char *signal, sharedCharArray_ptr _t1);
    bool connectDynamicSignal(const char *signal, QObject *obj, const char *slot);

    // bool connectDynamicSlot(QObject *obj, char *signal, char *slot);

public slots:
    //增加/删除关注代码
    virtual void addLive(mktCodeI_t);
    virtual void delLive(mktCodeI_t);

signals:
	void signal_L2Stats(const struct S4::NW::L2Stats_t&);
	void signal_L2Data(const NW::L2Data_arPtr_t&);
	void signal_L2Data_instrument_snap(const S4::sharedCharArray_ptr&);
	void signal_L2Data_index_snap(const S4::sharedCharArray_ptr&);
	void signal_L2Data_order(const S4::sharedCharArray_ptr&);
	void signal_L2Data_exec(const S4::sharedCharArray_ptr&);

protected:
    volatile bool m_stop;
    std::shared_ptr<NW::L2DataQ_t> _pL2DataQ;
    std::shared_ptr<NW::L2CmdQ_t> _pCmdQ;
    std::shared_ptr<moodycamel::ProducerToken> _pPtok_cmdQ;

protected:
    void run();
	
protected:
	DynamicQObject_mt _DynamicQObject;
	
};




}
}