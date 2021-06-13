#pragma once

#include "s4info.h"
#include "types/s4type.h"
#include "common/s4time.h"
#include "common/s4exceptions.h"
#include "db_sqlite/db.h"

namespace S4 {

#define SNAP_SIZE_LMT_MINU 5	//how many minus of snap need keep in memory
#define SNAP_SIZE_LMT_NB ((SNAP_SIZE_LMT_MINU)*60 /3)	//1 snapshot = 3 seconds

class infSnap_t : public info_t
{
public:
	infSnap_t():
		info_t()
	{}

	infSnap_t(const struct tdx_snap_t& quote, int date) :
		mktCode(quote.mktCode),
		active1(quote.active1),	//active1增加1，表示这期间发生过成交。如果没有成交，每次快照给出的时间戳、五档价格和委托量都是可能变动的，但active1不变。如果active1增加1，那么总量，总金额一定会变。
		price(quote.price),
		last_close(quote.last_close),
		open(quote.open),
		high(quote.high),
		low(quote.low),
		//timeS         (quote.timeS            ),
		vol(quote.vol),			//总量
		cur_vol(quote.cur_vol),		//现量
		amount(quote.amount),
		s_vol(quote.s_vol),
		b_vol(quote.b_vol),
		bid1(quote.bid1),
		ask1(quote.ask1),
		bid_vol1(quote.bid_vol1),
		ask_vol1(quote.ask_vol1),
		bid2(quote.bid2),
		ask2(quote.ask2),
		bid_vol2(quote.bid_vol2),
		ask_vol2(quote.ask_vol2),
		bid3(quote.bid3),
		ask3(quote.ask3),
		bid_vol3(quote.bid_vol3),
		ask_vol3(quote.ask_vol3),
		bid4(quote.bid4),
		ask4(quote.ask4),
		bid_vol4(quote.bid_vol4),
		ask_vol4(quote.ask_vol4),
		bid5(quote.bid5),
		ask5(quote.ask5),
		bid_vol5(quote.bid_vol5),
		ask_vol5(quote.ask_vol5),
		reversed_bytes0(quote.reversed_bytes0),
		reversed_bytes1(quote.reversed_bytes1),	//-价格*100
		reversed_bytes2(quote.reversed_bytes2),	//市场
		reversed_bytes3(quote.reversed_bytes3),
		reversed_bytes4(quote.reversed_bytes4),
		reversed_bytes5(quote.reversed_bytes5),
		reversed_bytes6(quote.reversed_bytes6),
		reversed_bytes7(quote.reversed_bytes7),
		reversed_bytes8(quote.reversed_bytes8),
		reversed_bytes9(quote.reversed_bytes9),        //涨速
		active2(quote.active2),	//活跃度
		lcl_time(quote.lcl_time)
	{
		_date = date;
		ms = quote.ms;	//这个时间可能不准，解析不对
        utc_to_date(quote.lcl_time, &lcl_minu);
		_MinmuSec = lcl_minu;	//用本地时间，tdx时间解析有问题
		_time = quote.lcl_time;// date_to_utc(_date, lcl_minu);	//用本地时间，tdx时间解析有问题
	}

	infSnap_t(const infSnap_t& quote) :
		mktCode(quote.mktCode),
		active1(quote.active1),	//active1增加1，表示这期间发生过成交。如果没有成交，每次快照给出的时间戳、五档价格和委托量都是可能变动的，但active1不变。如果active1增加1，那么总量，总金额一定会变。
		price(quote.price),
		last_close(quote.last_close),
		open(quote.open),
		high(quote.high),
		low(quote.low),
		//timeS         (quote.timeS            ),
		ms(quote.ms),
		vol(quote.vol),			//总量
		cur_vol(quote.cur_vol),		//现量
		amount(quote.amount),
		s_vol(quote.s_vol),
		b_vol(quote.b_vol),
		bid1(quote.bid1),
		ask1(quote.ask1),
		bid_vol1(quote.bid_vol1),
		ask_vol1(quote.ask_vol1),
		bid2(quote.bid2),
		ask2(quote.ask2),
		bid_vol2(quote.bid_vol2),
		ask_vol2(quote.ask_vol2),
		bid3(quote.bid3),
		ask3(quote.ask3),
		bid_vol3(quote.bid_vol3),
		ask_vol3(quote.ask_vol3),
		bid4(quote.bid4),
		ask4(quote.ask4),
		bid_vol4(quote.bid_vol4),
		ask_vol4(quote.ask_vol4),
		bid5(quote.bid5),
		ask5(quote.ask5),
		bid_vol5(quote.bid_vol5),
		ask_vol5(quote.ask_vol5),
		reversed_bytes0(quote.reversed_bytes0),
		reversed_bytes1(quote.reversed_bytes1),	//-价格*100
		reversed_bytes2(quote.reversed_bytes2),	//市场
		reversed_bytes3(quote.reversed_bytes3),
		reversed_bytes4(quote.reversed_bytes4),
		reversed_bytes5(quote.reversed_bytes5),
		reversed_bytes6(quote.reversed_bytes6),
		reversed_bytes7(quote.reversed_bytes7),
		reversed_bytes8(quote.reversed_bytes8),
		reversed_bytes9(quote.reversed_bytes9),        //涨速
		active2(quote.active2),	//活跃度
		lcl_time(quote.lcl_time),
        lcl_minu(quote.lcl_minu)
	{
		_date = quote._date;
		_MinmuSec = quote._MinmuSec;
		_time = quote._time;
	};


	infSnap_t& operator=(const infSnap_t& quote) {
		this->mktCode = quote.mktCode;
		this->active1 = quote.active1;	//active1增加1，表示这期间发生过成交。如果没有成交，每次快照给出的时间戳、五档价格和委托量都是可能变动的，但active1不变。如果active1增加1，那么总量，总金额一定会变。
		this->price = quote.price;
		this->last_close = quote.last_close;
		this->open = quote.open;
		this->high = quote.high;
		this->low = quote.low;
		//this->timeS          = quote.timeS            ;
		this->ms = quote.ms;
		this->vol = quote.vol;			//总量
		this->cur_vol = quote.cur_vol;		//现量
		this->amount = quote.amount;
		this->s_vol = quote.s_vol;
		this->b_vol = quote.b_vol;
		this->bid1 = quote.bid1;
		this->ask1 = quote.ask1;
		this->bid_vol1 = quote.bid_vol1;
		this->ask_vol1 = quote.ask_vol1;
		this->bid2 = quote.bid2;
		this->ask2 = quote.ask2;
		this->bid_vol2 = quote.bid_vol2;
		this->ask_vol2 = quote.ask_vol2;
		this->bid3 = quote.bid3;
		this->ask3 = quote.ask3;
		this->bid_vol3 = quote.bid_vol3;
		this->ask_vol3 = quote.ask_vol3;
		this->bid4 = quote.bid4;
		this->ask4 = quote.ask4;
		this->bid_vol4 = quote.bid_vol4;
		this->ask_vol4 = quote.ask_vol4;
		this->bid5 = quote.bid5;
		this->ask5 = quote.ask5;
		this->bid_vol5 = quote.bid_vol5;
		this->ask_vol5 = quote.ask_vol5;
		this->reversed_bytes0 = quote.reversed_bytes0;
		this->reversed_bytes1 = quote.reversed_bytes1;	//-价格*100
		this->reversed_bytes2 = quote.reversed_bytes2;	//市场
		this->reversed_bytes3 = quote.reversed_bytes3;
		this->reversed_bytes4 = quote.reversed_bytes4;
		this->reversed_bytes5 = quote.reversed_bytes5;
		this->reversed_bytes6 = quote.reversed_bytes6;
		this->reversed_bytes7 = quote.reversed_bytes7;
		this->reversed_bytes8 = quote.reversed_bytes8;
		this->reversed_bytes9 = quote.reversed_bytes9;        //涨速
		this->active2 = quote.active2;	//活跃度
		this->lcl_time = quote.lcl_time;
		this->lcl_minu = quote.lcl_minu;
		this->_date = quote._date;
		this->_MinmuSec = quote._MinmuSec;
		this->_time = quote._time;
		return *this;
	}

	float bidA_kw(void)const {
		return ((float)bid1*bid_vol1 + (float)bid2*bid_vol2 + (float)bid3*bid_vol3 + (float)bid4*bid_vol4 + (float)bid5*bid_vol5) / _KW;
	}
	//n=1~5
	float bidA_kw(int n) const {
		switch (n)
		{
		case 1:
			return (float)bid1*bid_vol1 / _KW;
		case 2:
			return (float)bid2*bid_vol2 / _KW;
		case 3:
			return (float)bid3*bid_vol3 / _KW;
		case 4:
			return (float)bid4*bid_vol4 / _KW;
		case 5:
			return (float)bid5*bid_vol5 / _KW;
		default:
			throw SNAP_Error("illegal to get bidA_kw("+std::to_string(n)+")");
		}
		return 0;
	}

	float askA_kw(void)const {
		return ((float)ask1*ask_vol1 + (float)ask2*ask_vol2 + (float)ask3*ask_vol3 + (float)ask4*ask_vol4 + (float)ask5*ask_vol5) / _KW;
	}
	//n=1~5
	float askA_kw(int n) const {
		switch (n)
		{
		case 1:
			return (float)ask1*ask_vol1 / _KW;
		case 2:
			return (float)ask2*ask_vol2 / _KW;
		case 3:
			return (float)ask3*ask_vol3 / _KW;
		case 4:
			return (float)ask4*ask_vol4 / _KW;
		case 5:
			return (float)ask5*ask_vol5 / _KW;
		default:
			throw SNAP_Error("illegal to get askA_kw(" + std::to_string(n) + ")");
		}
		return 0;
	}

	inline int avgPrice() const {
		return (int)(amount / vol);
	}

	inline int up20P() const {
		return UP_20p(last_close);
	}

	inline int dn20P() const {
		return DN_20p(last_close);
	}

	inline int up10P() const {
		return UP_10p(last_close);
	}

	inline int dn10P() const {
		return DN_10p(last_close);
	}
	
	/*upBan*/
	inline bool isOpenUpBan(void)const {
		return open == up10P();
	}

	inline bool isBidUpBan(void)const {
		return bid1 == up10P();
	}

	inline bool isPriceUpBan(void)const {
		return price == up10P();
	}

	inline bool isHighUpBan(void)const {
		return high == up10P();
	}

	inline bool isAskUpBan(void)const {
		return ask1 == up10P();
	}
	inline bool isAsk5UpBan(void)const {
		return ask5 == up10P();
	}

	/*dnBan*/
	inline bool isBidDnBan(void)const {
		return bid1 == dn10P();
	}
	inline bool isBid5DnBan(void)const {
		return bid5 == dn10P();
	}

	inline bool isPriceDnBan(void)const {
		return price == dn10P();
	}

	inline bool isLowDnBan(void)const {
		return low == dn10P();
	}

	inline bool isAskDnBan(void)const {
		return ask1 == dn10P();
	}


	inline float openR(void)const {
		return CALC_R_PERCENT(open, last_close);
	}

	inline float lowR(void)const {
		return CALC_R_PERCENT(low, last_close);
	}

	inline float highR(void)const {
		return CALC_R_PERCENT(high, last_close);
	}

	inline float priceR(void)const {
		return CALC_R_PERCENT(price, last_close);
	}

	inline int priceR_int(void)const {
		return CALC_R_PERCENT_INT(price, last_close);
	}
	
	inline float avgR(void)const {
		return CALC_R_PERCENT(amount / vol, last_close);
	}

	inline float bid1R(void)const {
		return CALC_R_PERCENT(bid1, last_close);
	}

	inline float amt_kw(void)const {
		return amount / _KW;
	}

	inline float curA_kw(void)const {
		return cur_vol * price / _KW;
	}

	inline float b1A_kw(void)const {
		return (float)bid1*bid_vol1 / _KW;
	}

	inline float a1A_kw(void)const {
		return (float)ask1*ask_vol1 / _KW;
	}

	inline int getAskVolOfPrice(int priceOfLevel)const {
		if (priceOfLevel == ask1) return ask_vol1;
		if (priceOfLevel == ask2) return ask_vol2;
		if (priceOfLevel == ask3) return ask_vol3;
		if (priceOfLevel == ask4) return ask_vol4;
		if (priceOfLevel == ask5) return ask_vol5;
		return 0;
	}

	inline int getBidVolOfPrice(int priceOfLevel)const {
		if (priceOfLevel == bid1) return bid_vol1;
		if (priceOfLevel == bid2) return bid_vol2;
		if (priceOfLevel == bid3) return bid_vol3;
		if (priceOfLevel == bid4) return bid_vol4;
		if (priceOfLevel == bid5) return bid_vol5;
		return 0;
	}

	inline int maxValidAsk(void)const {
		if (ask5 != 0) return ask5;
		if (ask4 != 0) return ask4;
		if (ask3 != 0) return ask3;
		if (ask2 != 0) return ask2;
		return ask1;	//no matter ask1 is 0 or not
	}

	inline int minValidBid(void)const {
		if (bid5 != 0) return bid5;
		if (bid4 != 0) return bid4;
		if (bid3 != 0) return bid3;
		if (bid2 != 0) return bid2;
		return bid1;	//no matter bid1 is 0 or not
	}

	int mktCode;
	uint16_t active1;			//active1增加1，表示这期间发生过成交。如果没有成交，每次快照给出的时间戳、五档价格和委托量都是可能变动的，但active1不变。如果active1增加1，那么总量，总金额一定会变。
	int price;
	int last_close;
	int open;
	int high;
	int low;
	// timeb timeS;
	int ms;
	int vol;					//总量
	int cur_vol;				//现量
	float amount = 0;
	int s_vol;
	int b_vol;
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

	int reversed_bytes0;
	int reversed_bytes1;		//-价格*100
	int reversed_bytes2;		//市场
	int reversed_bytes3;
	uint16_t reversed_bytes4;
	int reversed_bytes5;
	int reversed_bytes6;
	int reversed_bytes7;
	int reversed_bytes8;
	uint16_t reversed_bytes9;	//涨速
	uint16_t active2;			//活跃度
	time_t lcl_time;
	int lcl_minu;
};

typedef std::shared_ptr<infSnap_t> infSnap_ptr;

namespace sqlite {
class s4infSnap_tableIO;
}

class infSnapQ_t : public infQ_t<infSnap_t>
{
public:
	~infSnapQ_t();
	void add_snapshot(std::shared_ptr<infSnap_t> pSnapshot) {
		push_back(pSnapshot);
		if (size() > SNAP_SIZE_LMT_NB*2) {
			dropOld(SNAP_SIZE_LMT_NB);
		}
	};

	void add_quote(const tdx_snap_t& quote, int date) {
		if (size() != 0 && date == back()->_date && quote.active1 != 0 && quote.active1 < back()->active1)
			return;

		push_back(std::make_shared<infSnap_t>(quote, date));
		if (size() > SNAP_SIZE_LMT_NB*2) {
			dropOld(SNAP_SIZE_LMT_NB);
		}
	};
	//void calcFQ(const vec_gbbd_t* gbbd, const vec_fhps_t* fhps);
	virtual infSnap_t* getLastInfo(time_t clk);
	virtual infSnap_t* getLastInfo(time_t clk, int ref);

	void use_lcl_time(void);
	virtual void addQ(const infQ_t<infSnap_t>& Q, size_t lmt_size);
	virtual void addQ(const infQ_t<infSnap_t>& Q);

	std::vector<time_t> pullTicks(void);

	void setDB(bool bToDB, bool bFromDB, const std::string & dbPath) {
		_toDB = bToDB;
		_fromDB = bFromDB;
		_dbPath = dbPath;	//直到db文件
	}
	void set_dbTbl_name(const std::string&);
	const std::string & get_dbTbl_name(void) const { return _dbTbleName; }

	void toDB(void);
	void fromDB(int lmt_Qsize=0);
private:
	bool _toDB = false;
	bool _fromDB = false;
	bool _read_order_asc;
	std::string _dbPath;

	const time_t lastInfoMaxGap = 7200;

	std::shared_ptr<sqlite::s4infSnap_tableIO> _pTableIO = nullptr;
	//std::map<uint64_t, std::shared_ptr<infSnap_t>> _loadDBQ;
private:
	//void calcFH(const vec_fhps_t* fhps);
	//void calcGB(const vec_gbbd_t* gbbd);
	virtual void dropOld(size_t nb);

private:
	std::string _dbTbleName;
};

typedef std::shared_ptr<infSnapQ_t> infSnapQ_ptr;

/************************************************************************/
/* snap tools                                                           */
/************************************************************************/
struct snapVolAna_t
{
	int curAskVolDec;   //be Similar to traded ask vol since last to now (be bought)
	int curBidVolDec;	//be Similar to traded bid vol                   (be sold)
	int askDecAvgPrice;
	int askDecAvgPriceDiff;
	int bidDecAvgPrice;
	int bidDecAvgPriceDiff;
};
void snapVolAna(const infSnap_t& lastSnap, const infSnap_t& nowSnap, snapVolAna_t& volAna);



}