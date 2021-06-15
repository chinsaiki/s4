#include "qt_market/s4MarketDataAgent.h"
#include "qt_common/sharedCharArray_ptr.h"

#include "sbe_ssz.h"
#include "sbe_ssh.h"

#include <QDebug>
#include <QtGlobal>

using namespace S4::NW;

namespace S4{
namespace QT{

marketDataAgent::marketDataAgent(std::shared_ptr<NW::L2DataQ_t>& pL2DataQ, std::shared_ptr<NW::L2CmdQ_t>& pCmdQ, QObject *parent):
    QThread(parent),
    _DynamicQObject(this),
    _pL2DataQ(pL2DataQ),
    _pCmdQ(pCmdQ)
{
    m_stop = false;
    if (_pCmdQ){
        _pPtok_cmdQ = std::make_shared<moodycamel::ProducerToken>(*_pCmdQ);
    }
}

void marketDataAgent::run()
{
    m_stop = false;
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
		{
                    L2Stats_t stats = *(L2Stats_t*)pL2Data->pQdata->pBuffer;
                    emit signal_L2Stats(stats);
		}
                break;
            case L2DataType::MARKET_DATA:
                emit signal_L2Data(pL2Data);
                {
                    SBE_SSZ_header_t* pH = (SBE_SSZ_header_t*)pL2Data->pQdata->pBuffer;
                    sharedCharArray_ptr s = make_sharedCharArray_ptr(pH->MsgLen);
                    memcpy(s->get(), pL2Data->pQdata->pBuffer, pH->MsgLen);

                    int codeI = std::atoi(pH->SecurityID);
                    std::string securityID(pureCodeInt_to_pureCodeStr(codeI));
                    if (pH->SecurityIDSource==101){
                        securityID = "sh" + securityID;
                    }else if (pH->SecurityIDSource==102){
                        securityID = "sz" + securityID;
                    }
                    std::string signalName;

                    switch (pH->MsgType)
                    {
                    case __MsgType_SSZ_INDEX_SNAP__:
                        // emit signal_L2Data_index_snap(s);
                        signalName = "signal_L2Data_index_snap" + securityID + "(S4::sharedCharArray_ptr)";
                        if (emitDynamicSignal(signalName.data(), s)) {
                        }
                        break;
                    case __MsgType_SSZ_INSTRUMENT_SNAP__:
                        // emit signal_L2Data_instrument_snap(s);
						signalName = "signal_L2Data_instrument_snap" + securityID + "(S4::sharedCharArray_ptr)";
                        if (emitDynamicSignal(signalName.data(), s)) {
						}
                        break;
                    case __MsgType_SSZ_EXECUTION__:
                        // emit signal_L2Data_exec(s);
                        signalName = "signal_L2Data_exec" + securityID + "(S4::sharedCharArray_ptr)";
                        if (emitDynamicSignal(signalName.data(), s)) {
                        }
                        break;
                    case __MsgType_SSZ_ORDER__:
                        // emit signal_L2Data_order(s);
                        signalName = "signal_L2Data_order" + securityID + "(S4::sharedCharArray_ptr)";
                        if (emitDynamicSignal(signalName.data(), s)) {
                        }
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
}

    //增加/删除关注代码
void marketDataAgent::addLive(mktCodeI_t code)
{
    if (!_pPtok_cmdQ) return;
    std::shared_ptr<live_cmd_t> pData(new live_cmd_t);
    pData->add = true;
    pData->code = code;
    _pCmdQ->enqueue(*_pPtok_cmdQ, std::move(pData));
}

void marketDataAgent::delLive(mktCodeI_t code)
{
    if (!_pPtok_cmdQ) return;
    std::shared_ptr<live_cmd_t> pData(new live_cmd_t);
    pData->add = false;
    pData->code = code;
    _pCmdQ->enqueue(*_pPtok_cmdQ, std::move(pData));
}


// bool marketDataAgent::connectDynamicSlot(QObject *obj, char *signal, char *slot)
// {
// }

bool marketDataAgent::connectDynamicSignal(const char *signal, QObject *obj, const char *slot)
{
    return _DynamicQObject.connectDynamicSignal(signal, obj, slot);
}

bool marketDataAgent::disconnectDynamicSignal(const char* signal, QObject* obj, const char* slot)
{
	return _DynamicQObject.disconnectDynamicSignal(signal, obj, slot);
}


bool marketDataAgent::emitDynamicSignal(const char *signal, sharedCharArray_ptr _t1)
{
    return _DynamicQObject.emitDynamicSignal(signal, _t1);
}


}
}
