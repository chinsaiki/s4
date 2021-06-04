
#ifndef __SBE_SSZ_H__
#define __SBE_SSZ_H__
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

#define __MsgType_SSZ_INSTRUMENT_SNAP__         (111)
#define __MsgType_SSZ_INDEX_SNAP__              (11)
#define __MsgType_SSZ_ORDER__                   (192)
#define __MsgType_SSZ_EXECUTION__               (191)

//Stock ShenZhen Unified header
struct SBE_SSZ_header_t
{
    uint8_t     SecurityIDSource;   //=102
    uint8_t     MsgType;            //111, 11, 192, 191
    uint16_t    MsgLen;             //include this header, until ending byte of SBE message
    char        SecurityID[9];      // c8 + '\0'
    uint16_t    ChannelNo;          //
    uint64_t    ApplSeqNum;         //仅191和192有效
    uint8_t     TradingPhase;       //仅111和11有效：'1'=全天停牌; others=3xxx11.TradingPhaseCode[0]; 0x00=don't care;
}PACKED;


//map from MsgType=300111
struct SBE_SSZ_instrument_snap_t
{
    struct SBE_SSZ_header_t  Header;

    int64_t         NumTrades;
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
    int32_t         UpLimitPx;  //无涨停价格限制 = 0x7fffffff (转自999999999.999900)
    int32_t         DnLimitPx;  //无跌停价格限制 = 0x2710 (转自0.010000) 或 0x80000000 (转自-999999999.999900)
    struct price_level_t   BidLevel[10];
    struct price_level_t   AskLevel[10];
    uint64_t         TransactTime;
    uint8_t          Resv[4];
}PACKED;

//map from MsgType=309011
struct SBE_SSZ_index_snap_t
{
    struct SBE_SSZ_header_t  Header;

    int64_t         NumTrades;
    int64_t         TotalVolumeTrade;
    int64_t         TotalValueTrade;
    int64_t         PrevClosePx;
    int64_t         LastPx;
    int64_t         OpenPx;
    int64_t         HighPx;
    int64_t         LowPx;
    uint64_t        TransactTime;
    // int8_t          Resv[6];
}PACKED;

//map from MsgType=300192
struct SBE_SSZ_ord_t
{
    struct SBE_SSZ_header_t  Header;

    int32_t         Price;    //
    int64_t         OrderQty;
    int8_t          Side;       //
    int8_t          OrdType;
    uint64_t        TransactTime;
    uint8_t         Resv[2];
}PACKED;

//map from MsgType=300191
struct SBE_SSZ_exe_t
{
    struct SBE_SSZ_header_t  Header;

    int64_t         BidApplSeqNum;
    int64_t         OfferApplSeqNum;
    int32_t         LastPx;
    int64_t         LastQty;
    int8_t          ExecType;
    uint64_t         TransactTime;
    uint8_t          Resv[3];
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
#endif /*__SBE_SSZ_H__*/
