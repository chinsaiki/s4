#include "network/L2_udp_recver_th.h"
#include "common/s4time.h"
#include <cstring>

namespace S4{
namespace NW{

std::string L2Stats_t::toString() const
{
    std::string s;
    s += "recv_frame_cnt = " + std::to_string(recv_frame_cnt) + "\n";
    s += "recv_heartbeat_cnt = " + std::to_string(recv_heartbeat_cnt) + "\n";
    s += "recv_instrument_snap_cnt = " + std::to_string(recv_instrument_snap_cnt) + "\n";
    s += "recv_index_snap_cnt = " + std::to_string(recv_index_snap_cnt) + "\n";
    s += "recv_order_cnt = " + std::to_string(recv_order_cnt) + "\n";
    s += "recv_exec_cnt = " + std::to_string(recv_exec_cnt) + "\n";
    s += "recv_unknown_cnt = " + std::to_string(recv_unknown_cnt) + "\n";
    s += "last_frame_time_ms = " + std::to_string(last_frame_time_ms) + "\n";
    s += "live_code_nb = " + std::to_string(live_code_nb) + "\n";
    s += "live_instrument_snap_cnt = " + std::to_string(live_instrument_snap_cnt) + "\n";
    s += "live_index_snap_cnt = " + std::to_string(live_index_snap_cnt) + "\n";
    s += "live_order_cnt = " + std::to_string(live_order_cnt) + "\n";
    s += "live_exec_cnt = " + std::to_string(live_exec_cnt) + "\n";
    return s;
}



void L2_udp_recver_th::setReportInterval(unsigned int ms)
{
    _report_interval_ms = ms;
}


unsigned int L2_udp_recver_th::getReportInterval() const
{
    return _report_interval_ms;
}

void L2_udp_recver_th::report_stats(bool force)
{
    uint64_t now_ms = nowTime_ms();
    if (!force && now_ms < _last_report_time_ms + _report_interval_ms)
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
