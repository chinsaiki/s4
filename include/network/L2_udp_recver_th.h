#pragma

#include "common/s4mktCode.h"
#include "queue/simpleQ_mpmc_ar.h"

#include <stdint.h>
#include <set>

namespace S4 {
namespace NW {

enum class L2DataType{
    UNKNOWN = 0,
    MARKET_DATA,
    STATS_DATA,
    // SOCKET_STATUS,
};

struct L2DataInfo_t{
    int data_len = 0;
    uint64_t local_time_ms;
    L2DataType type = L2DataType::UNKNOWN;
};

struct L2Stats_t
{
    size_t recv_frame_cnt;
    size_t recv_heartbeat_cnt;
    size_t recv_instrument_snap_cnt;
    size_t recv_index_snap_cnt;
    size_t recv_order_cnt;
    size_t recv_exec_cnt;
    size_t recv_unknown_cnt;
    uint64_t last_frame_time_ms;
    
    size_t live_code_nb;
    size_t live_instrument_snap_cnt;
    size_t live_index_snap_cnt;
    size_t live_order_cnt;
    size_t live_exec_cnt;

    std::string toString() const;
};

typedef simpleQ_mpmc_ar_t<L2DataInfo_t> L2DataQ_t;
typedef simpleQ_mpmc_ar_t<L2DataInfo_t>::queParticle_arPtr_t L2Data_arPtr_t;

//UDP L2行情接收与转发线程基类
//从UDP端口接收L2行情数据
//向L2DataQ转发关注的指数或股票L2行情
//定时向L2DataQ发送统计数据
class L2_udp_recver_th
{
public:
    L2_udp_recver_th(std::shared_ptr<L2DataQ_t> pL2DataQ):
        _pL2DataQ(pL2DataQ)
    {}

    //创建socket，并启动监听线程
    virtual bool start(const char* pLocalIp, const uint16_t port) = 0;
    //终止监听线程，并关掉socket
    virtual bool stop() = 0;

    //增加/删除关注代码
    virtual void addLive(mktCodeI_t);
    virtual void delLive(mktCodeI_t);

    void setReportInterval(unsigned int ms);
    unsigned int getReportInterval() const;
protected:
    //发送统计报告，内部会检测时间间隔，force=ture则强制发送
    virtual void report_stats(bool force=false);

protected:
    std::shared_ptr<L2DataQ_t> _pL2DataQ;

    struct L2Stats_t _stats;

    std::set<mktCodeI_t> _live_list;

    struct live_cmd_t{
        bool add;
        mktCodeI_t code;
    };
    moodycamel::BlockingConcurrentQueue<live_cmd_t> _liveQ;

    unsigned int _report_interval_ms = 500;

    uint64_t _last_report_time_ms = 0;
};



}
}
