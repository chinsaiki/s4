#pragma once

#include "L2_udp_recver_th.h"
#include "sockutil.h"

namespace S4 {
namespace NW {


//UDP L2行情接收与转发线程 基于原生socket
class L2_udp_recver_th_native   : public L2_udp_recver_th
{
public:
    L2_udp_recver_th_native(const std::shared_ptr<L2DataQ_t>& pL2DataQ, const std::shared_ptr<L2CmdQ_t>& pCmdQ):
        L2_udp_recver_th(pL2DataQ, pCmdQ)
    {
        if (_pCmdQ){
            _pCtok_cmdQ = std::make_shared<moodycamel::ConsumerToken>(*_pCmdQ);
        }
    }

    //创建socket，并启动监听线程
    virtual bool start(const char* pLocalIp, const uint16_t port) override;
    //终止监听线程，并关掉socket
    virtual bool stop() override;
protected:
    bool _running = false;
    bool _stop = false;
private:
    void recv_thread();
    bool liveTrans(char* pH);

    std::shared_ptr<moodycamel::ConsumerToken> _pCtok_cmdQ;
    void proc_cmdQ();

    int _fd;
    std::shared_ptr<std::thread> _pThread;
    std::mutex _mux;
};



}
}
