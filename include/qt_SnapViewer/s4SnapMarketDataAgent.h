#pragma once

#include "network/L2_udp_recver_th.h"
#include "qt_common/dynamicqobject.h"
#include <QThread>


namespace S4{
namespace QT{

    class s4SnapMarketDataAgent;
class AgentDynamicQObject : public DynamicQObject
{
public:
    AgentDynamicQObject(s4SnapMarketDataAgent *parent);

    DynamicSlot* createSlot(char*) { return NULL; }  //not use

private:
    s4SnapMarketDataAgent * app;
};

class s4SnapMarketDataAgent : public QThread
{
    Q_OBJECT
public:
    s4SnapMarketDataAgent(std::shared_ptr<NW::L2DataQ_t>& pL2DataQ, std::shared_ptr<NW::L2CmdQ_t>& pCmdQ, QObject *parent = 0);

    void stop()
    {
        m_stop = true;
    }


    bool emitDynamicSignal(char *signal, void **arguments);	
    bool connectDynamicSignal(char *signal, QObject *obj, char *slot);

    // bool connectDynamicSlot(QObject *obj, char *signal, char *slot);

public slots:
    //增加/删除关注代码
    virtual void addLive(mktCodeI_t);
    virtual void delLive(mktCodeI_t);

signals:
	void signal_L2Stats(const struct NW::L2Stats_t&);
	void signal_L2Data(const NW::L2Data_arPtr_t&);
	void signal_L2Data_instrument_snap(const std::string&);
	void signal_L2Data_index_snap(const std::string&);
	void signal_L2Data_order(const std::string&);
	void signal_L2Data_exec(const std::string&);

protected:
    volatile bool m_stop;
    std::shared_ptr<NW::L2DataQ_t> _pL2DataQ;
    std::shared_ptr<NW::L2CmdQ_t> _pCmdQ;
    std::shared_ptr<moodycamel::ProducerToken> _pPtok_cmdQ;

protected:
    void run();
	
protected:
	AgentDynamicQObject _DynamicQObject;
	
};




}
}