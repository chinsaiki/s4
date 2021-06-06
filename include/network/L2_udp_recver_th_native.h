#pragma

#include "L2_udp_recver_th.h"
#include "sockutil.h"

namespace S4 {
namespace NW {


//UDP L2行情接收与转发线程 基于原生socket
class L2_udp_recver_th_native   : public L2_udp_recver_th
{
public:
    L2_udp_recver_th_native(std::shared_ptr<L2DataQ_t> pL2DataQ):
        L2_udp_recver_th(pL2DataQ)
    {}

    //创建socket，并启动监听线程
    virtual bool start(const char* pLocalIp, const uint16_t port) override;
    //终止监听线程，并关掉socket
    virtual bool stop() override;
protected:
    bool _running = false;
    bool _stop = false;
private:
    void recv_thread(const char* pLocalIp, const uint16_t port);
    bool liveTrans(char* pH);

    int _fd;
    std::shared_ptr<std::thread> _pThread;
    std::mutex _mux;
};



}
}
