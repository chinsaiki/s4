#include "network/L2_udp_recver_th.h"
#include "common/s4time.h"
namespace S4{
namespace NW{


//增加/删除关注代码
void L2_udp_recver_th::addLive(mktCodeI_t code)
{
    live_cmd_t data;
    data.add = true;
    data.code = code;
    _liveQ.enqueue(std::move(data));
}
void L2_udp_recver_th::delLive(mktCodeI_t code)
{
    live_cmd_t data;
    data.add = false;
    data.code = code;
    _liveQ.enqueue(std::move(data));
}

void L2_udp_recver_th::setReportInterval(unsigned int ms)
{
    _report_interval_ms = ms;
}

void L2_udp_recver_th::report_stats()
{
    uint64_t now_ms = nowTime_ms();
    if (now_ms < _last_report_time_ms + _report_interval_ms)
        return;

    L2Data_arPtr_t report_data;
    _pL2DataQ->P_get_tryBest(report_data);
    report_data->pQdata->info.type = L2DataType::STATS_DATA;
    report_data->pQdata->info.local_time_ms = now_ms;
    report_data->pQdata->info.data_len = sizeof(L2Stats_t);
    memcpy(report_data->pQdata->pBuffer, &_stats, sizeof(L2Stats_t));

    _last_report_time_ms = now_ms;
}

}
}
