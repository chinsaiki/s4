#pragma once

#include <time.h>
#include <stdint.h>
#include <sys/timeb.h>
#include <vector>
#include <string>
#include <map>
#include <list>

#include "s4type_precision.h"
#include "s4type_time.h"
#include "common/s4mktCode.h"	//types of code

// #include "s3type_future.h"

namespace S4{

/************************************************************************/
/* WARNING: Vol in DayK/minuK/GBBQ is in Gu(1 share), Vol in Snap is in Shou(1 hand) = 100 Gu!*/
/************************************************************************/
typedef std::vector<struct minuK_t> vec_minuK_t;
struct minuK_t
{
	time_utcSec_t time;	//utcSec
	int32_t open;
	int32_t high;
	int32_t low;
	int32_t close;
	float amount;	//yuan
	uint32_t volume;	//gu (1 share)
};

typedef std::vector<struct dayK_t> vec_dayK_t;
struct dayK_t
{
	time_date_t date;			//YYYYMMDD
	int32_t open;
	int32_t high;
	int32_t low;
	int32_t close;
	float amount;		//yuan
	uint32_t volume;		//gu (1 share)
	uint32_t reserve;		//300etf在20150706时成交量超出uint32最大值，此时此字段最高位为ff，表示volume需乘以100。
};


typedef std::vector<struct gbbd_t> vec_gbbd_t;
typedef std::vector<struct fhps_t> vec_fhps_t;
struct gbbd_t
{
	//unsigned char isSz;
	uint32_t strDate;
	//unsigned char category; //信息类型:1的为除权除息, 大于1的为股本变动
	float qianliutong;		//前总流通(万股) -> gu
	float qianzong;			//前总股本
	float houliutong;		//后总流通
	float houzong;			//后总股本
};
struct fhps_t
{
	//unsigned char isSz;
	uint32_t strDate;
	//unsigned char category; //信息类型:1的为除权除息, 大于1的为股本变动
	float hongli;		//每10股派几元 -> 1股
	float peigujia;		//配股价		
	float songgu;		//每10股送几股 -> 1股
	float peigu;		//每10股配几股 -> 1股
};


//typedef std::map<int, vec_gbbd_t> map_gbbd_t;
//typedef std::map<int, vec_fhps_t> map_fhps_t;
class map_gbbd_t : public std::map<int, vec_gbbd_t>
{
public:
	const vec_gbbd_t* getStk(const std::string&) const;
	const vec_gbbd_t* getStk(int) const;

};

class map_fhps_t : public std::map<int, vec_fhps_t>
{
public:
	const vec_fhps_t* getStk(const std::string&) const;
	const vec_fhps_t* getStk(int) const;

};


enum runMode_t
{
	rOTF = 0,
	rREG,
	rCap,
};

enum timeMode_t
{
	tDAY = 0,
	tMINU,
	tSnap,
	//tTick,

	tUNKNOWN,
};

////////////
enum trade_opt_t
{
	oSEND_OPEN = 0,
	oCHANGE_CTS,
	oABORT_OPEN,
	oOPEN,
	oSEND_CLOSE,
	oCHANGE_OTS,
	oABORT_CLOSE,
	oCLOSE,
	oTAKE,
	oSTOP,
	oUNKNOWN_OPT
};
std::string trade_opt_t_toString(trade_opt_t t);
trade_opt_t trade_opt_t_fromString(const std::string & str);


////////////
enum orderType_t {
	UNKNOW = 0,
	LIMITED_BUY,
	LIMITED_SELL,
	MARKET_BUY,
	MARKET_SELL,
	BUY,
	SELL,
	ANY,
	CANCEL_BUY,
	CANCEL_SELL,
	//MARKET_CLOSE,
	//LIMITED_CLOSE,
};

std::string orderTypeStr(orderType_t t);
orderType_t str2orderType(const std::string & str);

bool isOrderTypeEqual(orderType_t t0, orderType_t t1);

//*:                         AUTO
//BUY:  dbp / RZ_BUY(first n) |   NORMAL_STOCK
//SELL: dbp / RQ_SELL(TODO)   |   NORMAL_STOCK
//CANCEL: RZRQ                |   NORMAL_STOCK
enum tradeType_t
{
	AUTO = 0,
	NORMAL_STOCK,
	DBP,
	RZRQ,
	RZ_BUY,
	RQ_SELL,

};
std::string tradeTypeStr(tradeType_t t);


enum marketType_t
{
	STOCK_AUTO = 0,
	STOCK_RZ,
	STOCK_DBP,
	STOCK_STK
};
std::string marketTypeStr(marketType_t t);
marketType_t str2marketType(const std::string & str);


//humanOrder
struct horder_t
{
	int mktCode;
	int vol;
	int openPrice;
	marketType_t marketType;
	orderType_t orderType;
	std::string source;
	std::string toString(void) const;
};

}//namespace S4

