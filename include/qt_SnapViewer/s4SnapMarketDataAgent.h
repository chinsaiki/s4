#pragma once

#include "network/L2_udp_recver_th.h"
#include "qt_common/dynamicqobject.h"
#include <QThread>


namespace S4{
namespace QT{

class s4SnapMarketDataAgent : public QThread//, public DynamicQObject
{
    Q_OBJECT
public:
    s4SnapMarketDataAgent(std::shared_ptr<NW::L2DataQ_t>& pL2DataQ, std::shared_ptr<NW::L2CmdQ_t>& pCmdQ):
        _pL2DataQ(pL2DataQ),
        _pCmdQ(pCmdQ)
    {
        m_stop = false;
        if (_pCmdQ){
            _pPtok_cmdQ = std::make_shared<moodycamel::ProducerToken>(*_pCmdQ);
        }
    }
    void stop()
    {
        m_stop = true;
    }

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
};

}
}