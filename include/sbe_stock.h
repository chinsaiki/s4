#ifndef __SBE_STOCK_H__
#define __SBE_STOCK_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#ifdef _MSC_VER
#define PACKED
#pragma pack(push,1)
#else
#define PACKED __attribute__ ((__packed__))
#endif


struct price_level_t
{
    int32_t    Price;  //price
    int64_t    Qty;    //qty
}PACKED;

struct QtyQueue_level_t   //not imple for now
{
    uint8_t  NoOrders;  //nb of QtyQueue
    uint16_t QtyQueue[50];
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
#endif /*__SBE_STOCK_H__*/
