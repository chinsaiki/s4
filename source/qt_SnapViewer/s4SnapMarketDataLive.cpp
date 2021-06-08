#include "qt_SnapViewer/s4SnapMarketDataLive.h"
#include "sbe_ssz.h"
#include "sbe_ssh.h"

#include <QDebug>

using namespace S4::NW;

namespace S4{
namespace QT{

void s4SnapMarketDataLive::run()
{
    m_stop = false;
    qDebug() << "run begin";
    std::vector<L2Data_arPtr_t> pv;
    bool got_data;
    while (!m_stop)
    {
        got_data = _pL2DataQ->C_getBulk_timeout(pv, 64, 100 * 1000);
        if (!got_data)
            continue;
        for (auto &pL2Data : pv)
        {
            switch (pL2Data->pQdata->info.type)
            {
            case L2DataType::STATS_DATA:
                L2Stats_t stats = *(L2Stats_t*)pL2Data->pQdata->pBuffer;
                emit signal_L2Stats(stats);
                break;
            case L2DataType::MARKET_DATA:
                emit signal_L2Data(pL2Data);
                {
                    SBE_SSZ_header_t* pH = (SBE_SSZ_header_t*)pL2Data->pQdata->pBuffer;
                    std::string s(pL2Data->pQdata->pBuffer, pH->MsgLen);

                    // QString securityID(pH->SecurityID);
                    // if (pH->SecurityIDSource==101){
                    //     securityID = "sh" + securityID;
                    // }else if (pH->SecurityIDSource==102){
                    //     securityID = "sz" + securityID;
                    // }
                    // QString signalName(pH->SecurityID);
                    // QVector<void *> args(2, 0);

                    switch (pH->MsgType)
                    {
                    case __MsgType_SSZ_INDEX_SNAP__:
                        emit signal_L2Data_index_snap(s);
                        // signalName = "signal_L2Data_index_snap" + securityID + "(std::string)";
                        // args[0] = 0;
                        // args[1] = &s;    
                        // emitDynamicSignal(signalName.toStdString().data(), args.data());
                        break;
                    case __MsgType_SSZ_INSTRUMENT_SNAP__:
                        emit signal_L2Data_instrument_snap(s);
                        // signalName = "signal_L2Data_instrument_snap" + securityID;
                        // args[0] = 0;
                        // args[1] = &s;    
                        // emitDynamicSignal(signalName.toStdString().data(), args.data());
                        break;
                    case __MsgType_SSZ_EXECUTION__:
                        emit signal_L2Data_exec(s);
                        break;
                    case __MsgType_SSZ_ORDER__:
                        emit signal_L2Data_order(s);
                        break;
                    default:
                        break;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
    qDebug() << "run end";
}

    //增加/删除关注代码
void s4SnapMarketDataLive::addLive(mktCodeI_t code)
{
    if (!_pPtok_cmdQ) return;
    std::shared_ptr<live_cmd_t> pData(new live_cmd_t);
    pData->add = true;
    pData->code = code;
    _pCmdQ->enqueue(*_pPtok_cmdQ, std::move(pData));
}

void s4SnapMarketDataLive::delLive(mktCodeI_t code)
{
    if (!_pPtok_cmdQ) return;
    std::shared_ptr<live_cmd_t> pData(new live_cmd_t);
    pData->add = false;
    pData->code = code;
    _pCmdQ->enqueue(*_pPtok_cmdQ, std::move(pData));
}

 //void s4SnapMarketDataLive::onL2Stats(const struct L2Stats_t&)
 //{}
 //void s4SnapMarketDataLive::onL2Data(const NW::L2Data_arPtr_t&)
 //{}
 //void s4SnapMarketDataLive::onL2Data_instrument_snap(const std::string&)
 //{}
 //void s4SnapMarketDataLive::onL2Data_index_snap(const std::string&)
 //{}
 //void s4SnapMarketDataLive::onL2Data_order(const std::string&)
 //{}
 //void s4SnapMarketDataLive::onL2Data_exec(const std::string&)
 //{}

}
}
