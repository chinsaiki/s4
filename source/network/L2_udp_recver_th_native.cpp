#include "network/L2_udp_recver_th_native.h"
#include "queue/shared_array.h"
#include "common/s4time.h"
#include "sbe_ssz.h"
#include "sbe_ssh.h"

namespace S4
{
namespace NW
{

bool L2_udp_recver_th_native::start(const char* pLocalIp, const uint16_t port)
{
    std::lock_guard<std::mutex> l(_mux);
    if (_pThread)
        return false;   //no dual start

    memset(&_stats, 0, sizeof(_stats));

    _pThread = std::make_shared<std::thread>(
        [&](const char* ip, const uint16_t pt){
            recv_thread(ip, pt);
        }, 
        pLocalIp, port
    );

    return true;
}

bool L2_udp_recver_th_native::stop()
{
    std::lock_guard<std::mutex> l(_mux);
    if (_pThread){
        _stop = true;
        _pThread->join();
        _pThread.reset();
        _stop = false;
    }

    return true;
}

void L2_udp_recver_th_native::recv_thread(const char* pLocalIp, const uint16_t port)
{
    _fd = SockUtil::bindUdpSock(port, pLocalIp);
    if (_fd < 0)
        return;

    char_array_t recv_buffer(4096);
    int recv_len;
    do {
        recv_len = recv(_fd, recv_buffer.get(), (int)recv_buffer.size(), 0);
        if (recv_len > 0){
            _stats.recv_frame_cnt++;
            _stats.last_frame_time_ms = nowTime_ms();

            //统计类型，检查是否需要转发
            int len = recv_len;
            SBE_SSH_header_t* pH = (SBE_SSH_header_t*)recv_buffer.get();
            
            do{
                switch (pH->MsgType)
                {
                case __MsgType_SSH_INSTRUMENT_SNAP__:
                    if ((pH->SecurityIDSource == 101 && pH->MsgLen == sizeof(SBE_SSH_instrument_snap_t) && len >= (int)sizeof(SBE_SSH_instrument_snap_t)) ||
                        (pH->SecurityIDSource == 102 && pH->MsgLen == sizeof(SBE_SSZ_instrument_snap_t) && len >= (int)sizeof(SBE_SSZ_instrument_snap_t))) {
                        _stats.recv_instrument_snap_cnt++;
                        _stats.live_instrument_snap_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        pH += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_INDEX_SNAP__:
                    if ((pH->SecurityIDSource == 101 && pH->MsgLen == sizeof(SBE_SSH_index_snap_t) && len >= (int)sizeof(SBE_SSH_index_snap_t)) ||
                        (pH->SecurityIDSource == 102 && pH->MsgLen == sizeof(SBE_SSZ_index_snap_t) && len >= (int)sizeof(SBE_SSZ_index_snap_t))) {
                        _stats.recv_index_snap_cnt++;
                        _stats.live_index_snap_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        pH += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_ORDER__:
                    if ((pH->SecurityIDSource == 101 && pH->MsgLen == sizeof(SBE_SSH_ord_t) && len >= (int)sizeof(SBE_SSH_ord_t)) ||
                        (pH->SecurityIDSource == 102 && pH->MsgLen == sizeof(SBE_SSZ_ord_t) && len >= (int)sizeof(SBE_SSZ_ord_t))) {
                        _stats.recv_order_cnt++;
                        _stats.live_order_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        pH += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_SSH_EXECUTION__:
                    if ((pH->SecurityIDSource == 101 && pH->MsgLen == sizeof(SBE_SSH_exe_t) && len >= (int)sizeof(SBE_SSH_exe_t)) ||
                        (pH->SecurityIDSource == 102 && pH->MsgLen == sizeof(SBE_SSZ_exe_t) && len >= (int)sizeof(SBE_SSZ_exe_t))) {
                        _stats.recv_exec_cnt++;
                        _stats.live_exec_cnt += liveTrans((char*)pH);
                        len -= pH->MsgLen;
                        pH += pH->MsgLen;
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                case __MsgType_HEARTBEAT__:
                    if ((pH->SecurityIDSource == 101 || pH->SecurityIDSource == 102) && pH->MsgLen == sizeof(SBE_SSH_header_t) && len >= (int)sizeof(SBE_SSH_header_t)){
                        _stats.recv_heartbeat_cnt++;
                        len -= sizeof(SBE_SSH_header_t);
                        pH += sizeof(SBE_SSH_header_t);
                    }else{
                        _stats.recv_unknown_cnt++;
                        len = 0;
                    }
                    break;
                
                default:
                    _stats.recv_unknown_cnt++;
                    len = 0;
                    break;
                }
            }while(len > 0);
        }else if(recv_len == -1){
            report_stats();
        }else if (recv_len < -1){
            break;
        }
    } while (!_stop);

    report_stats(true);
}

bool L2_udp_recver_th_native::liveTrans(char* pH)
{
    mktCodeI_t code;
    code = atoi(((SBE_SSH_header_t*)pH)->SecurityID);
    if (((SBE_SSH_header_t*)pH)->SecurityIDSource == 101){
        code += SH_PRB;
    }else{
        code += SZ_PRB;
    }
    if (_live_list.count(code)){
        L2Data_arPtr_t p;
        _pL2DataQ->P_get_tryBest(p);
        p->pQdata->info.data_len = ((SBE_SSH_header_t*)pH)->MsgLen;
        p->pQdata->info.local_time_ms = _stats.last_frame_time_ms;
        p->pQdata->info.type = L2DataType::MARKET_DATA;
        memcpy(p->pQdata->pBuffer, pH, ((SBE_SSH_header_t*)pH)->MsgLen);
        return true;
    }
    return false;
}



} // namespace NW
} // namespace S4
