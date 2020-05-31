#include "s3type.h"
#include "s3codeApp.h"
#include "s3convertors.h"
#include "s3app.h"
#include "s3exceptions.h"

using namespace std;

string stk_tdx_quote_t::toString() const
{
	int day, minu;
	day = utc_to_date(timeS.time, &minu);
	return move(
		to_string(minu) + "." + to_string(timeS.millitm) +
		": mktCode= " + to_string(mktCode) +
		"; active1= " + to_string(active1) +
		"; price= " + to_string(price) +
		"; last_close= " + to_string(last_close) +
		"; open= " + to_string(open) +
		"; high= " + to_string(high) +
		"; low= " + to_string(low) +
		"; reversed_bytes0= " + to_string(reversed_bytes0) +
		"; reversed_bytes1= " + to_string(reversed_bytes1) +
		"; vol= " + to_string(vol) +
		"; cur_vol= " + to_string(cur_vol) +
		"; amount= " + to_string(amount) +
		"; s_vol= " + to_string(s_vol) +
		"; b_vol= " + to_string(b_vol) +
		"; reversed_bytes2= " + to_string(reversed_bytes2) +
		"; reversed_bytes3= " + to_string(reversed_bytes3) +
		"; bid1= " + to_string(bid1) +
		"; ask1= " + to_string(ask1) +
		"; bid_vol1= " + to_string(bid_vol1) +
		"; ask_vol1= " + to_string(ask_vol1) +
		"; bid2= " + to_string(bid2) +
		"; ask2= " + to_string(ask2) +
		"; bid_vol2= " + to_string(bid_vol2) +
		"; ask_vol2= " + to_string(ask_vol2) +
		"; bid3= " + to_string(bid3) +
		"; ask3= " + to_string(ask3) +
		"; bid_vol3= " + to_string(bid_vol3) +
		"; ask_vol3= " + to_string(ask_vol3) +
		"; bid4= " + to_string(bid4) +
		"; ask4= " + to_string(ask4) +
		"; bid_vol4= " + to_string(bid_vol4) +
		"; ask_vol4= " + to_string(ask_vol4) +
		"; bid5= " + to_string(bid5) +
		"; ask5= " + to_string(ask5) +
		"; bid_vol5= " + to_string(bid_vol5) +
		"; ask_vol5= " + to_string(ask_vol5) +
		"; reversed_bytes4= " + to_string(reversed_bytes4) +
		"; reversed_bytes5= " + to_string(reversed_bytes5) +
		"; reversed_bytes6= " + to_string(reversed_bytes6) +
		"; reversed_bytes7= " + to_string(reversed_bytes7) +
		"; reversed_bytes8= " + to_string(reversed_bytes8) +
		"; reversed_bytes9= " + to_string(reversed_bytes9) +
		"; active2= " + to_string(active2) +
		"\n");
}

int cnvStk(const std::string& code)
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
	int icode = cnvStk(code);
	if(this->count(icode)!=0)
		return &at(cnvStk(code));
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
	int icode = cnvStk(code);
	if(this->count(icode)!=0)
		return &at(cnvStk(code));
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

std::string orderTypeStr(orderType_t t)
{
	std::string s( 
		t == UNKNOW ? "UNKNOW" :
		t == LIMITED_BUY ? "LIMITED_BUY" :
		t == LIMITED_SELL ? "LIMITED_SELL" :
		t == MARKET_BUY ? "MARKET_BUY" :
		t == MARKET_SELL ? "MARKET_SELL" :
		t == BUY ? "BUY" :
		t == SELL ? "SELL" :
		t == ANY ? "ANY" :
		t == CANCEL_BUY ? "CANCEL_BUY" :
		t == CANCEL_SELL ? "CANCEL_SELL" :
		"ILLEGAL"
	);
	return move(s);
}

orderType_t str2orderType(const std::string & str)
{
	if (string_toUpper(str) == "UNKNOW") return UNKNOW;
	if (string_toUpper(str) == "LIMITED_BUY") return LIMITED_BUY;
	if (string_toUpper(str) == "LIMITED_SELL") return LIMITED_SELL;
	if (string_toUpper(str) == "MARKET_BUY") return MARKET_BUY;
	if (string_toUpper(str) == "MARKET_SELL") return MARKET_SELL;
	if (string_toUpper(str) == "BUY") return BUY;
	if (string_toUpper(str) == "SELL") return SELL;
	if (string_toUpper(str) == "ANY") return ANY;
	if (string_toUpper(str) == "CANCEL_BUY") return CANCEL_BUY;
	if (string_toUpper(str) == "CANCEL_SELL") return CANCEL_SELL;

	throw type_Error("str2marketType: illegal str=" + str);
}


bool isOrderTypeEqual(orderType_t t0, orderType_t t1) {
	if (t0 == t1)
		return true;

	if (t0 == BUY && (t1 == MARKET_BUY || t1 == LIMITED_BUY))
		return true;
	if (t1 == BUY && (t0 == MARKET_BUY || t0 == LIMITED_BUY))
		return true;

	if (t0 == SELL && (t1 == MARKET_SELL || t1 == LIMITED_SELL))
		return true;
	if (t1 == SELL && (t0 == MARKET_SELL || t0 == LIMITED_SELL))
		return true;

	if (t0 == ANY || t1 == ANY)
		return true;

	return false;
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
	if (string_toUpper(str) == "STOCK_AUTO") return STOCK_AUTO;
	if (string_toUpper(str) == "STOCK_RZ") return STOCK_RZ;
	if (string_toUpper(str) == "STOCK_DBP") return STOCK_DBP;
	if (string_toUpper(str) == "STOCK_STK") return STOCK_STK;
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
