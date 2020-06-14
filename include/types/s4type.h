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
#include "market/s4codeApp.h"	//types of code

// #include "s3type_future.h"

/************************************************************************/
/* WARNING: Vol in DayK/minuK/GBBQ is in Gu(1 share), Vol in Snap is in Shou(1 hand) = 100 Gu!*/
/************************************************************************/
typedef std::vector<struct minuK_t> vec_minuK_t;
struct minuK_t
{
	time_utcSec_t time;	//utcSec
	price_t open;
	price_t high;
	price_t low;
	price_t close;
	amount_t amount;	//yuan
	uint32_t volume;	//gu (1 share)
};

typedef std::vector<struct dayK_t> vec_dayK_t;
struct dayK_t
{
	time_date_t date;			//YYYYMMDD
	price_t open;
	price_t high;
	price_t low;
	price_t close;
	amount_t amount;		//yuan
	uint32_t volume;		//gu (1 share)
	price_t preClose;		//nothing inside, but need here for tdx-file-reading
};

struct stk_tdx_quote_t
{
	int mktCode;
	uint16_t active1;			//active1����1����ʾ���ڼ䷢�����ɽ������û�гɽ���ÿ�ο��ո�����ʱ������嵵�۸��ί�������ǿ��ܱ䶯�ģ���active1���䡣���active1����1����ô�������ܽ��һ����䡣
	int price;
	int last_close;
	int open;
	int high;
	int low;
	int reversed_bytes0;
	timeb timeS;
	int reversed_bytes1;		//-�۸�*100
	int vol;					//���� ��
	int cur_vol;				//���� �� index?
	amount_t amount;
	int s_vol;
	int b_vol;
	int reversed_bytes2;		//�г�
	int reversed_bytes3;
	int bid1;
	int ask1;
	int bid_vol1;
	int ask_vol1;
	int bid2;
	int ask2;
	int bid_vol2;
	int ask_vol2;
	int bid3;
	int ask3;
	int bid_vol3;
	int ask_vol3;
	int bid4;
	int ask4;
	int bid_vol4;
	int ask_vol4;
	int bid5;
	int ask5;
	int bid_vol5;
	int ask_vol5;
	uint16_t reversed_bytes4;
	int reversed_bytes5;
	int reversed_bytes6;
	int reversed_bytes7;
	int reversed_bytes8;
	uint16_t reversed_bytes9;	//����
	uint16_t active2;			//��Ծ��
	time_t lcl_time;

	std::string toString(void) const;
};

typedef std::vector<struct gbbd_t> vec_gbbd_t;
typedef std::vector<struct fhps_t> vec_fhps_t;
struct gbbd_t
{
	//unsigned char isSz;
	uint32_t strDate;
	//unsigned char category; //��Ϣ����:1��Ϊ��Ȩ��Ϣ, ����1��Ϊ�ɱ��䶯
	float qianliutong;		//ǰ����ͨ(���) -> gu
	float qianzong;			//ǰ�ܹɱ�
	float houliutong;		//������ͨ
	float houzong;			//���ܹɱ�
};
struct fhps_t
{
	//unsigned char isSz;
	uint32_t strDate;
	//unsigned char category; //��Ϣ����:1��Ϊ��Ȩ��Ϣ, ����1��Ϊ�ɱ��䶯
	float hongli;		//ÿ10���ɼ�Ԫ -> 1��
	float peigujia;		//��ɼ�		
	float songgu;		//ÿ10���ͼ��� -> 1��
	float peigu;		//ÿ10���伸�� -> 1��
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
};

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



