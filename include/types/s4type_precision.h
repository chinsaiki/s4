#pragma once

//TDX 整数价格，精度=分
typedef int32_t price_t;
typedef double fprice_t;
#define iPrice_precision 100

//float price to int price: 15.199999 -> 15.20499 -> 1520
#define fPrice_to_iPrice(x) ((price_t)(((x)+0.005)*iPrice_precision))
#define iPrice_to_fPrice(x) ((x)*(1.0/iPrice_precision))

//交易所L2 逐笔价格精度
#define L2_iPrice_tick_precision 10000
//交易所L2 快照价格精度
#define L2_iPrice_snap_precision 1000000
//交易所L2 数量精度
#define L2_Qty_precision 100
//交易所L2 成交量精度
#define L2_Amt_precision 10000

#define L2_iPrice_tick_to_fPrice(x) ((x)*(1.0/L2_iPrice_tick_precision))
#define L2_iPrice_snap_to_fPrice(x) ((x)*(1.0/L2_iPrice_snap_precision))
#define L2_Qty_to_hand(x)   ((x)/(100.0*L2_Qty_precision))

//5% 板价格
#define UP_5p(x) ((price_t)((x)*1.05+0.5))
#define DN_5p(x) ((price_t)((x)*0.95+0.5))

//10% 板价格
#define UP_10p(x) ((price_t)((x)*1.1+0.5))
#define DN_10p(x) ((price_t)((x)*0.9+0.5))

//20% 板价格
#define UP_20p(x) ((price_t)((x)*1.2+0.5))
#define DN_20p(x) ((price_t)((x)*0.8+0.5))

//涨幅 %
#define CALC_R_PERCENT(x,y) ((x) * 100.0 / (y) - 100.0)

//涨幅 千分比
#define CALC_R_PERMILL(x,y) ((x) * 1000.0 / (y) - 1000.0)

//涨幅 % 整数计算
#define CALC_R_PERCENT_INT(x,y) ((x) * 100 / (y) - 100)

//成交额，精度=元
typedef float amount_t;

//for amount
#define _KW ((amount_t)1E7)

//成交量，DayK/minuK/GBBQ中精度=股(share)，snap中精度=手(board)=100股
typedef long long vol_share_t;
typedef long long vol_board_t;
#define vBoard_unit (100)
// #define vShare_to_vBoard(x)  ((x)/vBoard_unit)
#define vBoard_to_vShare(x)  ((x)*vBoard_unit)

//tdx快照 成交量精度
typedef int32_t vol_tdx_t;

//换手率
typedef double turnover_t;

//复权
typedef double fq_index_t;
typedef double fq_share_t;


#define _MAX_(x,y) (((x)>(y))?(x):(y))
#define _MIN_(x,y) (((x)<(y))?(x):(y))
#define _ABS_(x) ((x)<0?-(x):(x))
