
#ifndef __SBE_SSH_H__
#define __SBE_SSH_H__
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "sbe_stock.h"

#ifdef _MSC_VER
#define PACKED
#pragma pack(push,1)
#else
#define PACKED __attribute__ ((__packed__))
#endif

#ifndef __MsgType_HEARTBEAT__
#define __MsgType_HEARTBEAT__               (1)
#endif

#define __MsgType_SSH_INSTRUMENT_SNAP__         (111)
#define __MsgType_SSH_INDEX_SNAP__              (11)
#define __MsgType_SSH_ORDER__                   (192)
#define __MsgType_SSH_EXECUTION__               (191)

//Stock ShangHai Unified header
struct SBE_SSH_header_t
{
    uint8_t     SecurityIDSource;   //=101
    uint8_t     MsgType;            //111, 11, 192, 191
    uint16_t    MsgLen;             //include this header, until ending byte of SBE message
    char        SecurityID[9];      // c8 + '\0'
    uint16_t    ChannelNo;          //
    uint64_t    ApplSeqNum;         //仅191和192有效
    uint8_t     TradingPhase;       //'1'=不可正常交易或未上市或当期阶段不接受订单申报; others=UA3202.TradingPhaseCode[0]; 0x00=don't care;
}PACKED;

//map from MsgType=UA3202
struct SBE_SSH_instrument_snap_t
{
    struct SBE_SSH_header_t  Header;

    int32_t         NumTrades;
    int64_t         TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int32_t         PrevClosePx;
    int32_t         LastPx;
    int32_t         OpenPx;
    int32_t         HighPx;
    int32_t         LowPx;
    int32_t         BidWeightPx;
    int64_t         BidWeightSize;
    int32_t         AskWeightPx;
    int64_t         AskWeightSize;
    //int32_t         UpLimitPx;  // SH 无此字段
    //int32_t         DnLimitPx;  // SH 无此字段
    struct price_level_t   BidLevel[10];
    struct price_level_t   AskLevel[10];
    uint32_t         DataTimeStamp; //秒
    uint8_t          Resv[4];
}PACKED;

//map from MsgType=UA3113
struct SBE_SSH_index_snap_t
{
    struct SBE_SSH_header_t  Header;    //msgType=111

    int64_t         TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int64_t         PrevClosePx;
    int64_t         LastPx;
    int64_t         OpenPx;
    int64_t         HighPx;
    int64_t         LowPx;
    uint64_t        DataTimeStamp;
    // int8_t          Resv[6];
}PACKED;

//map from MsgType=UA5801
struct SBE_SSH_ord_t
{
    struct SBE_SSH_header_t  Header;    //msgType=11

    int64_t         OrderNo;    //
    int32_t         Price;      //
    int64_t         OrderQty;
    int8_t          Side;       //'B', 'S'
    int8_t          OrdType;    //'A', 'D'
    uint32_t        OrderTime;  //委托时间（百分之一秒）14302506 表示 14:30:25.06
    uint8_t         Resv[2];
}PACKED;

//map from MsgType=UA3201
struct SBE_SSH_exe_t
{
    struct SBE_SSH_header_t  Header;

    int64_t         TradeBuyNo;
    int64_t         TradeSellNo;
    int32_t         LastPx;
    int64_t         LastQty;
    int8_t          TradeBSFlag;    //'B', 'S', 'N'
    uint64_t        TradeTime;
    uint8_t         Resv[3];
}PACKED;


#ifdef _MSC_VER
#pragma pack(pop)
#undef PACKED
#else
#undef PACKED
#endif


#ifdef __cplusplus
}
#endif
#endif /*__SBE_SSH_H__*/
