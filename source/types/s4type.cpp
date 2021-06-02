//#include "s3type.h"
//#include "s3codeApp.h"
//#include "s3convertors.h"
//#include "s3app.h"
//#include "s3exceptions.h"
#include "types/s4type.h"
#include "types/s4convertors.h"
#include "common/s4string.h"
#include "common/s4time.h"
#include "common/s4mktCode.h"
#include <string>

using namespace std;

namespace S4{

string stk_tdx_snap_toString(const tdx_snap_t& snap)
{
	int minu;
	utc_to_date(snap.time, &minu);
	return move(
		to_string(minu) + "." + to_string(snap.ms) +
		": mktCode= " + to_string(snap.mktCode) +
		"; active1= " + to_string(snap.active1) +
		"; price= " + to_string(snap.price) +
		"; last_close= " + to_string(snap.last_close) +
		"; open= " + to_string(snap.open) +
		"; high= " + to_string(snap.high) +
		"; low= " + to_string(snap.low) +
		"; reversed_bytes0= " + to_string(snap.reversed_bytes0) +
		"; reversed_bytes1= " + to_string(snap.reversed_bytes1) +
		"; vol= " + to_string(snap.vol) +
		"; cur_vol= " + to_string(snap.cur_vol) +
		"; amount= " + to_string(snap.amount) +
		"; s_vol= " + to_string(snap.s_vol) +
		"; b_vol= " + to_string(snap.b_vol) +
		"; reversed_bytes2= " + to_string(snap.reversed_bytes2) +
		"; reversed_bytes3= " + to_string(snap.reversed_bytes3) +
		"; bid1= " + to_string(snap.bid1) +
		"; ask1= " + to_string(snap.ask1) +
		"; bid_vol1= " + to_string(snap.bid_vol1) +
		"; ask_vol1= " + to_string(snap.ask_vol1) +
		"; bid2= " + to_string(snap.bid2) +
		"; ask2= " + to_string(snap.ask2) +
		"; bid_vol2= " + to_string(snap.bid_vol2) +
		"; ask_vol2= " + to_string(snap.ask_vol2) +
		"; bid3= " + to_string(snap.bid3) +
		"; ask3= " + to_string(snap.ask3) +
		"; bid_vol3= " + to_string(snap.bid_vol3) +
		"; ask_vol3= " + to_string(snap.ask_vol3) +
		"; bid4= " + to_string(snap.bid4) +
		"; ask4= " + to_string(snap.ask4) +
		"; bid_vol4= " + to_string(snap.bid_vol4) +
		"; ask_vol4= " + to_string(snap.ask_vol4) +
		"; bid5= " + to_string(snap.bid5) +
		"; ask5= " + to_string(snap.ask5) +
		"; bid_vol5= " + to_string(snap.bid_vol5) +
		"; ask_vol5= " + to_string(snap.ask_vol5) +
		"; reversed_bytes4= " + to_string(snap.reversed_bytes4) +
		"; reversed_bytes5= " + to_string(snap.reversed_bytes5) +
		"; reversed_bytes6= " + to_string(snap.reversed_bytes6) +
		"; reversed_bytes7= " + to_string(snap.reversed_bytes7) +
		"; reversed_bytes8= " + to_string(snap.reversed_bytes8) +
		"; reversed_bytes9= " + to_string(snap.reversed_bytes9) +
		"; active2= " + to_string(snap.active2) +
		"\n");
}

static
int stk_strCode2mktCode(const std::string& code)
{
	int ret = static_cast<int>(IntConvertor::convert(code.substr(2)));
	if (code.substr(0, 2) == "sh")
	{
		ret += SH_PRB;
	}
	else if (code.substr(0, 2) == "sz") {
		ret += SZ_PRB;
	}
	return ret;
}

const vec_gbbd_t* map_gbbd_t::getStk(const std::string& code) const
{
	int icode = stk_strCode2mktCode(code);
	if(this->count(icode)!=0)
		return &at(stk_strCode2mktCode(code));
	else
		return NULL;
}

const vec_gbbd_t* map_gbbd_t::getStk(int icode) const
{
	if(this->count(icode)!=0)
		return &at(icode);
	else
		return NULL;
}

const vec_fhps_t* map_fhps_t::getStk(const std::string& code) const
{
	int icode = stk_strCode2mktCode(code);
	if(this->count(icode)!=0)
		return &at(stk_strCode2mktCode(code));
	else
		return NULL;
}

const vec_fhps_t* map_fhps_t::getStk(int icode) const
{
	if(this->count(icode)!=0)
		return &at(icode);
	else
		return NULL;
}


////////////////
std::string trade_opt_t_toString(trade_opt_t t)
{
	std::string s( 
		t == trade_opt_t::oSEND_OPEN ? "oSEND_OPEN" :
		t == trade_opt_t::oOPEN ? "oOPEN" :
		t == trade_opt_t::oABORT_OPEN ? "oABORT_OPEN" :
		t == trade_opt_t::oSEND_CLOSE ? "oSEND_CLOSE" :
		t == trade_opt_t::oCLOSE ? "oCLOSE" :
		t == trade_opt_t::oABORT_CLOSE ? "oABORT_CLOSE" :
		t == trade_opt_t::oTAKE ? "oTAKE" :
		t == trade_opt_t::oSTOP ? "oSTOP" :
		t == trade_opt_t::oCHANGE_OTS ? "oCHANGE_OTS" :
		t == trade_opt_t::oCHANGE_CTS ? "oCHANGE_CTS" :
		"oUNKNOWN_OPT"
	);
	return move(s);
}

trade_opt_t trade_opt_t_fromString(const std::string & str)
{
	if (str == "oSEND_OPEN") return trade_opt_t::oSEND_OPEN;
	if (str == "oOPEN") return trade_opt_t::oOPEN;
	if (str == "oABORT_OPEN") return trade_opt_t::oABORT_OPEN;
	if (str == "oSEND_CLOSE") return trade_opt_t::oSEND_CLOSE;
	if (str == "oCLOSE") return trade_opt_t::oCLOSE;
	if (str == "oABORT_CLOSE") return trade_opt_t::oABORT_CLOSE;
	if (str == "oTAKE") return trade_opt_t::oTAKE;
	if (str == "oSTOP") return trade_opt_t::oSTOP;
	if (str == "oCHANGE_OTS") return trade_opt_t::oCHANGE_OTS;
	if (str == "oCHANGE_CTS") return trade_opt_t::oCHANGE_CTS;
	if (str == "oUNKNOWN_OPT") return trade_opt_t::oUNKNOWN_OPT;

	throw type_Error("trade_opt_t_fromString: illegal str=" + str);
}

////////////////
std::string orderTypeStr(orderType_t t)
{
	std::string s( 
		t == orderType_t::UNKNOW ? "UNKNOW" :
		t == orderType_t::LIMITED_BUY ? "LIMITED_BUY" :
		t == orderType_t::LIMITED_SELL ? "LIMITED_SELL" :
		t == orderType_t::MARKET_BUY ? "MARKET_BUY" :
		t == orderType_t::MARKET_SELL ? "MARKET_SELL" :
		t == orderType_t::BUY ? "BUY" :
		t == orderType_t::SELL ? "SELL" :
		t == orderType_t::ANY ? "ANY" :
		t == orderType_t::CANCEL_BUY ? "CANCEL_BUY" :
		t == orderType_t::CANCEL_SELL ? "CANCEL_SELL" :
		"ILLEGAL"
	);
	return move(s);
}

orderType_t str2orderType(const std::string & str)
{
	if (string_toUpper(str) == "UNKNOW") return orderType_t::UNKNOW;
	if (string_toUpper(str) == "LIMITED_BUY") return orderType_t::LIMITED_BUY;
	if (string_toUpper(str) == "LIMITED_SELL") return orderType_t::LIMITED_SELL;
	if (string_toUpper(str) == "MARKET_BUY") return orderType_t::MARKET_BUY;
	if (string_toUpper(str) == "MARKET_SELL") return orderType_t::MARKET_SELL;
	if (string_toUpper(str) == "BUY") return orderType_t::BUY;
	if (string_toUpper(str) == "SELL") return orderType_t::SELL;
	if (string_toUpper(str) == "ANY") return orderType_t::ANY;
	if (string_toUpper(str) == "CANCEL_BUY") return orderType_t::CANCEL_BUY;
	if (string_toUpper(str) == "CANCEL_SELL") return orderType_t::CANCEL_SELL;

	throw type_Error("str2marketType: illegal str=" + str);
}


bool isOrderTypeEqual(orderType_t t0, orderType_t t1) {
	if (t0 == t1)
		return true;

	if (t0 == orderType_t::BUY && (t1 == orderType_t::MARKET_BUY || t1 == orderType_t::LIMITED_BUY))
		return true;
	if (t1 == orderType_t::BUY && (t0 == orderType_t::MARKET_BUY || t0 == orderType_t::LIMITED_BUY))
		return true;

	if (t0 == orderType_t::SELL && (t1 == orderType_t::MARKET_SELL || t1 == orderType_t::LIMITED_SELL))
		return true;
	if (t1 == orderType_t::SELL && (t0 == orderType_t::MARKET_SELL || t0 == orderType_t::LIMITED_SELL))
		return true;

	if (t0 == orderType_t::ANY || t1 == orderType_t::ANY)
		return true;

	return false;
}


////////////////
std::string orderSideStr(orderSide_t t)
{
	std::string s( 
		t == orderSide_t::UNKNOW ? "UNKNOW" :
		t == orderSide_t::BID ? "BID" :
		t == orderSide_t::ASK ? "ASK" :
		"ILLEGAL"
	);
	return move(s);
}

orderSide_t str2orderSide(const std::string & str)
{
	if (string_toUpper(str) == "UNKNOW") return orderSide_t::UNKNOW;
	if (string_toUpper(str) == "BID") return orderSide_t::BID;
	if (string_toUpper(str) == "ASK") return orderSide_t::ASK;

	throw type_Error("str2marketSide: illegal str=" + str);
}


std::string tradeTypeStr(tradeType_t t)
{
	std::string s(
		t == tradeType_t::AUTO ? "AUTO" :
		t == tradeType_t::NORMAL_STOCK ? "NORMAL_STOCK" :
		t == tradeType_t::DBP ? "DBP" :
		t == tradeType_t::RZRQ ? "RZRQ" :
		t == tradeType_t::RZ_BUY ? "RZ_BUY" :
		t == tradeType_t::RQ_SELL ? "RQ_SELL" :
		"ILLEGAL"
	);
	return move(s);
}

std::string marketTypeStr(marketType_t t)
{
	std::string s(
		t == marketType_t::STOCK_AUTO ? "STOCK_AUTO" :
		t == marketType_t::STOCK_RZ ? "STOCK_RZ" :
		t == marketType_t::STOCK_DBP ? "STOCK_DBP" :
		t == marketType_t::STOCK_STK ? "STOCK_STK" :
		"ILLEGAL"
	);
	return move(s);
}

marketType_t str2marketType(const std::string & str)
{
	if (string_toUpper(str) == "STOCK_AUTO") return marketType_t::STOCK_AUTO;
	if (string_toUpper(str) == "STOCK_RZ") return marketType_t::STOCK_RZ;
	if (string_toUpper(str) == "STOCK_DBP") return marketType_t::STOCK_DBP;
	if (string_toUpper(str) == "STOCK_STK") return marketType_t::STOCK_STK;
	throw type_Error("str2marketType: illegal str=" + str);
}


std::string horder_t::toString(void) const {
	std::string s;
	s = mktCodeInt_to_mktCodeStr(mktCode) + "\t"
		+ "vol=" + std::to_string(vol) + "\t"
		+ "openPrice=" + std::to_string(openPrice) + "\t"
		+ "marketType=" + marketTypeStr(marketType) + "\t"
		+ "orderType=" + orderTypeStr(orderType) + "\t"
		+ "source=" + source;
	return std::move(s);
}

}//namespace S4
