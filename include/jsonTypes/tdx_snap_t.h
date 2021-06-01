/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: ./json_template\tdx_snap_t.json
* Type name: tdx_snap_t
* Tester:    int tdx_snap_t_tester();
*
* Json keep-word: 
    "__default_value_fields__": [], # Take value in .json file as the default value of cpp variable
    "__optional_fields__": [], # Not require to present to .json file, but always in cpp struct
    "__assign_type_fields__": {"field":"cpp-type"}, # Assign specal cpp-type of field, but not infer automatically as default.
    "__assign_enum_fields__": {"field":"enum-type"}, # Assign specal enum-type of field, but not infer automatically as default.
                              enum-type need have implemented <enum-type>_toSting() & <enum-type>_fromString() functions.
    "__assign_set_lists__": [], # Take list in .json file as std::set<>, but not std::vector<> as default
    "__comment__xxx":"", # Add comment line
    "__sqlite_capable__":"", # enable sqlite tableIO autogen
    "__sqlite_primary__":"", # assign the primary key of sqlite, if not assigned, first existing col of [ 'id', 'date', 'mktCode', 'datetime', 'code'] will be assigned automatically.
* Script author: ChinSaiki<chinsaiki@outlook.com>
*/
#pragma once


#include <assert.h>
#include "common/s4json_util.h"
#include "common/s4logger.h"
#include "types/s4type.h"

#include <set>
#include <list>
#include <vector>

namespace S4 {

/* type */
struct tdx_snap_t {
	time_date_t date;	//	0
	time_minuSec_t minuSec;	//	0
	time_utcSec_t time;	//	0
	mktCodeI_t mktCode;	//	0
	//active1增加1，表示这期间发生过成交。如果没有成交，每次快照给出的时间戳、五档价格和委托量都是可能变动的，但active1不变。如果active1增加1，那么总量，总金额一定会变。
	uint16_t active1;	//	0
	price_t price;	//	0
	price_t last_close;	//	0
	price_t open;	//	0
	price_t high;	//	0
	price_t low;	//	0
	int ms;	//	0
	//总量
	vol_tdx_t vol;	//	0
	//现量
	vol_tdx_t cur_vol;	//	0
	amount_t amount;	//	0.0
	vol_tdx_t s_vol;	//	0
	vol_tdx_t b_vol;	//	0
	price_t bid1;	//	0
	price_t ask1;	//	0
	vol_tdx_t bid_vol1;	//	0
	vol_tdx_t ask_vol1;	//	0
	price_t bid2;	//	0
	price_t ask2;	//	0
	vol_tdx_t bid_vol2;	//	0
	vol_tdx_t ask_vol2;	//	0
	price_t bid3;	//	0
	price_t ask3;	//	0
	vol_tdx_t bid_vol3;	//	0
	vol_tdx_t ask_vol3;	//	0
	price_t bid4;	//	0
	price_t ask4;	//	0
	vol_tdx_t bid_vol4;	//	0
	vol_tdx_t ask_vol4;	//	0
	price_t bid5;	//	0
	price_t ask5;	//	0
	vol_tdx_t bid_vol5;	//	0
	vol_tdx_t ask_vol5;	//	0
	uint32_t reversed_bytes0;	//	0
	//-价格*100
	uint32_t reversed_bytes1;	//	0
	//市场
	uint32_t reversed_bytes2;	//	0
	uint32_t reversed_bytes3;	//	0
	uint16_t reversed_bytes4;	//	0
	uint32_t reversed_bytes5;	//	0
	uint32_t reversed_bytes6;	//	0
	uint32_t reversed_bytes7;	//	0
	uint32_t reversed_bytes8;	//	0
	//涨速
	uint16_t reversed_bytes9;	//	0
	//活跃度
	uint16_t active2;	//	0
	time_utcSec_t lcl_time;	//	0
	std::string lcl_time_str;	//	
	int VERSION;	//	0

	/* from json */
	static bool from_json(const nlohmann::json& json_var, tdx_snap_t& tdx_snap_t_var){
		try{
			try{
				tdx_snap_t_var.date = json_var.at("date").get<time_date_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "date", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.minuSec = json_var.at("minuSec").get<time_minuSec_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "minuSec", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.time = json_var.at("time").get<time_utcSec_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "time", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.mktCode = json_var.at("mktCode").get<mktCodeI_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "mktCode", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.active1 = json_var.at("active1").get<uint16_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "active1", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.price = json_var.at("price").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "price", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.last_close = json_var.at("last_close").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "last_close", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.open = json_var.at("open").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "open", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.high = json_var.at("high").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "high", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.low = json_var.at("low").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "low", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ms = json_var.at("ms").get<int>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ms", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.vol = json_var.at("vol").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "vol", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.cur_vol = json_var.at("cur_vol").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "cur_vol", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.amount = json_var.at("amount").get<amount_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "amount", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.s_vol = json_var.at("s_vol").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "s_vol", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.b_vol = json_var.at("b_vol").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "b_vol", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid1 = json_var.at("bid1").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid1", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask1 = json_var.at("ask1").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask1", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid_vol1 = json_var.at("bid_vol1").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid_vol1", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask_vol1 = json_var.at("ask_vol1").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask_vol1", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid2 = json_var.at("bid2").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid2", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask2 = json_var.at("ask2").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask2", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid_vol2 = json_var.at("bid_vol2").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid_vol2", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask_vol2 = json_var.at("ask_vol2").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask_vol2", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid3 = json_var.at("bid3").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid3", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask3 = json_var.at("ask3").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask3", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid_vol3 = json_var.at("bid_vol3").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid_vol3", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask_vol3 = json_var.at("ask_vol3").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask_vol3", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid4 = json_var.at("bid4").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid4", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask4 = json_var.at("ask4").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask4", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid_vol4 = json_var.at("bid_vol4").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid_vol4", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask_vol4 = json_var.at("ask_vol4").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask_vol4", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid5 = json_var.at("bid5").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid5", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask5 = json_var.at("ask5").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask5", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.bid_vol5 = json_var.at("bid_vol5").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bid_vol5", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.ask_vol5 = json_var.at("ask_vol5").get<vol_tdx_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "ask_vol5", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes0 = json_var.at("reversed_bytes0").get<uint32_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes0", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes1 = json_var.at("reversed_bytes1").get<uint32_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes1", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes2 = json_var.at("reversed_bytes2").get<uint32_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes2", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes3 = json_var.at("reversed_bytes3").get<uint32_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes3", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes4 = json_var.at("reversed_bytes4").get<uint16_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes4", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes5 = json_var.at("reversed_bytes5").get<uint32_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes5", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes6 = json_var.at("reversed_bytes6").get<uint32_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes6", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes7 = json_var.at("reversed_bytes7").get<uint32_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes7", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes8 = json_var.at("reversed_bytes8").get<uint32_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes8", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.reversed_bytes9 = json_var.at("reversed_bytes9").get<uint16_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reversed_bytes9", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.active2 = json_var.at("active2").get<uint16_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "active2", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.lcl_time = json_var.at("lcl_time").get<time_utcSec_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "lcl_time", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.lcl_time_str = json_var.at("lcl_time_str").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "lcl_time_str", e.what());
				throw e;
			}
			try{
				tdx_snap_t_var.VERSION = json_var.at("VERSION").get<int>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "VERSION", e.what());
				throw e;
			}
		}catch (const std::exception& e){
			ERR("parse json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
	/* to json */
	static bool to_json(nlohmann::json& json_var, const tdx_snap_t& tdx_snap_t_var){
		try{
			json_var["date"] = tdx_snap_t_var.date;
			json_var["minuSec"] = tdx_snap_t_var.minuSec;
			json_var["time"] = tdx_snap_t_var.time;
			json_var["mktCode"] = tdx_snap_t_var.mktCode;
			json_var["active1"] = tdx_snap_t_var.active1;
			json_var["price"] = tdx_snap_t_var.price;
			json_var["last_close"] = tdx_snap_t_var.last_close;
			json_var["open"] = tdx_snap_t_var.open;
			json_var["high"] = tdx_snap_t_var.high;
			json_var["low"] = tdx_snap_t_var.low;
			json_var["ms"] = tdx_snap_t_var.ms;
			json_var["vol"] = tdx_snap_t_var.vol;
			json_var["cur_vol"] = tdx_snap_t_var.cur_vol;
			json_var["amount"] = tdx_snap_t_var.amount;
			json_var["s_vol"] = tdx_snap_t_var.s_vol;
			json_var["b_vol"] = tdx_snap_t_var.b_vol;
			json_var["bid1"] = tdx_snap_t_var.bid1;
			json_var["ask1"] = tdx_snap_t_var.ask1;
			json_var["bid_vol1"] = tdx_snap_t_var.bid_vol1;
			json_var["ask_vol1"] = tdx_snap_t_var.ask_vol1;
			json_var["bid2"] = tdx_snap_t_var.bid2;
			json_var["ask2"] = tdx_snap_t_var.ask2;
			json_var["bid_vol2"] = tdx_snap_t_var.bid_vol2;
			json_var["ask_vol2"] = tdx_snap_t_var.ask_vol2;
			json_var["bid3"] = tdx_snap_t_var.bid3;
			json_var["ask3"] = tdx_snap_t_var.ask3;
			json_var["bid_vol3"] = tdx_snap_t_var.bid_vol3;
			json_var["ask_vol3"] = tdx_snap_t_var.ask_vol3;
			json_var["bid4"] = tdx_snap_t_var.bid4;
			json_var["ask4"] = tdx_snap_t_var.ask4;
			json_var["bid_vol4"] = tdx_snap_t_var.bid_vol4;
			json_var["ask_vol4"] = tdx_snap_t_var.ask_vol4;
			json_var["bid5"] = tdx_snap_t_var.bid5;
			json_var["ask5"] = tdx_snap_t_var.ask5;
			json_var["bid_vol5"] = tdx_snap_t_var.bid_vol5;
			json_var["ask_vol5"] = tdx_snap_t_var.ask_vol5;
			json_var["reversed_bytes0"] = tdx_snap_t_var.reversed_bytes0;
			json_var["reversed_bytes1"] = tdx_snap_t_var.reversed_bytes1;
			json_var["reversed_bytes2"] = tdx_snap_t_var.reversed_bytes2;
			json_var["reversed_bytes3"] = tdx_snap_t_var.reversed_bytes3;
			json_var["reversed_bytes4"] = tdx_snap_t_var.reversed_bytes4;
			json_var["reversed_bytes5"] = tdx_snap_t_var.reversed_bytes5;
			json_var["reversed_bytes6"] = tdx_snap_t_var.reversed_bytes6;
			json_var["reversed_bytes7"] = tdx_snap_t_var.reversed_bytes7;
			json_var["reversed_bytes8"] = tdx_snap_t_var.reversed_bytes8;
			json_var["reversed_bytes9"] = tdx_snap_t_var.reversed_bytes9;
			json_var["active2"] = tdx_snap_t_var.active2;
			json_var["lcl_time"] = tdx_snap_t_var.lcl_time;
			json_var["lcl_time_str"] = tdx_snap_t_var.lcl_time_str;
			json_var["VERSION"] = tdx_snap_t_var.VERSION;
		}catch (const std::exception& e){
		ERR("to json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}

	bool operator ==(const tdx_snap_t& d) const
	{
		if (date == d.date &&
			minuSec == d.minuSec &&
			time == d.time &&
			mktCode == d.mktCode &&
			active1 == d.active1 &&
			price == d.price &&
			last_close == d.last_close &&
			open == d.open &&
			high == d.high &&
			low == d.low &&
			ms == d.ms &&
			vol == d.vol &&
			cur_vol == d.cur_vol &&
			amount == d.amount &&
			s_vol == d.s_vol &&
			b_vol == d.b_vol &&
			bid1 == d.bid1 &&
			ask1 == d.ask1 &&
			bid_vol1 == d.bid_vol1 &&
			ask_vol1 == d.ask_vol1 &&
			bid2 == d.bid2 &&
			ask2 == d.ask2 &&
			bid_vol2 == d.bid_vol2 &&
			ask_vol2 == d.ask_vol2 &&
			bid3 == d.bid3 &&
			ask3 == d.ask3 &&
			bid_vol3 == d.bid_vol3 &&
			ask_vol3 == d.ask_vol3 &&
			bid4 == d.bid4 &&
			ask4 == d.ask4 &&
			bid_vol4 == d.bid_vol4 &&
			ask_vol4 == d.ask_vol4 &&
			bid5 == d.bid5 &&
			ask5 == d.ask5 &&
			bid_vol5 == d.bid_vol5 &&
			ask_vol5 == d.ask_vol5 &&
			reversed_bytes0 == d.reversed_bytes0 &&
			reversed_bytes1 == d.reversed_bytes1 &&
			reversed_bytes2 == d.reversed_bytes2 &&
			reversed_bytes3 == d.reversed_bytes3 &&
			reversed_bytes4 == d.reversed_bytes4 &&
			reversed_bytes5 == d.reversed_bytes5 &&
			reversed_bytes6 == d.reversed_bytes6 &&
			reversed_bytes7 == d.reversed_bytes7 &&
			reversed_bytes8 == d.reversed_bytes8 &&
			reversed_bytes9 == d.reversed_bytes9 &&
			active2 == d.active2 &&
			lcl_time == d.lcl_time &&
			lcl_time_str == d.lcl_time_str &&
			VERSION == d.VERSION)
		{
			return true;
		}
		return false;
	}

	bool operator !=(const tdx_snap_t& d) const
	{
		return !((*this)==d);
	}

};// struct tdx_snap_t
} // namespace S4


        /* Tester */
        inline int tdx_snap_t_tester() {

            //std::ifstream i("G:/work2t/99_s3/s4/./json_template/tdx_snap_t.json");
            std::string i("{    \"__sqlite_capable__\" : true,    \"__sqlite_primary__\" : \"time\",    \"__assign_type_fields__\": {        \"date\":\"time_date_t\",        \"minuSec\":\"time_minuSec_t\",        \"time\":\"time_utcSec_t\",        \"mktCode\":\"mktCodeI_t\",        \"active1\": \"uint16_t\",        \"price\": \"price_t\",        \"last_close\": \"price_t\",        \"open\": \"price_t\",        \"high\": \"price_t\",        \"low\": \"price_t\",        \"vol\": \"vol_tdx_t\",        \"cur_vol\": \"vol_tdx_t\",        \"amount\": \"amount_t\",        \"s_vol\": \"vol_tdx_t\",        \"b_vol\": \"vol_tdx_t\",        \"bid1\":\"price_t\",        \"ask1\":\"price_t\",        \"bid_vol1\":\"vol_tdx_t\",        \"ask_vol1\":\"vol_tdx_t\",        \"bid2\":\"price_t\",        \"ask2\":\"price_t\",        \"bid_vol2\":\"vol_tdx_t\",        \"ask_vol2\":\"vol_tdx_t\",        \"bid3\":\"price_t\",        \"ask3\":\"price_t\",        \"bid_vol3\":\"vol_tdx_t\",        \"ask_vol3\":\"vol_tdx_t\",        \"bid4\":\"price_t\",        \"ask4\":\"price_t\",        \"bid_vol4\":\"vol_tdx_t\",        \"ask_vol4\":\"vol_tdx_t\",        \"bid5\":\"price_t\",        \"ask5\":\"price_t\",        \"bid_vol5\":\"vol_tdx_t\",        \"ask_vol5\":\"vol_tdx_t\",        \"reversed_bytes0\":\"uint32_t\",        \"reversed_bytes1\":\"uint32_t\",        \"reversed_bytes2\":\"uint32_t\",        \"reversed_bytes3\":\"uint32_t\",        \"reversed_bytes4\":\"uint16_t\",        \"reversed_bytes5\":\"uint32_t\",        \"reversed_bytes6\":\"uint32_t\",        \"reversed_bytes7\":\"uint32_t\",        \"reversed_bytes8\":\"uint32_t\",        \"reversed_bytes9\":\"uint16_t\",        \"active2\":\"uint16_t\",        \"lcl_time\":\"time_utcSec_t\"    },	\"date\":0,	\"minuSec\":0,	\"time\":0,	\"mktCode\": 0,    \"__comment__0\": \"active1增加1，表示这期间发生过成交。如果没有成交，每次快照给出的时间戳、五档价格和委托量都是可能变动的，但active1不变。如果active1增加1，那么总量，总金额一定会变。\",	\"active1\": 0,				\"price\":0,	\"last_close\":0,	\"open\":0,	\"high\":0,	\"low\":0,	\"ms\":0,    \"__comment__1\": \"总量\",	\"vol\":0,    \"__comment__2\": \"现量\",	\"cur_vol\":0,	\"amount\": 0.0,	\"s_vol\":0,	\"b_vol\":0,	\"bid1\":0,	\"ask1\":0,	\"bid_vol1\":0,	\"ask_vol1\":0,	\"bid2\":0,	\"ask2\":0,	\"bid_vol2\":0,	\"ask_vol2\":0,	\"bid3\":0,	\"ask3\":0,	\"bid_vol3\":0,	\"ask_vol3\":0,	\"bid4\":0,	\"ask4\":0,	\"bid_vol4\":0,	\"ask_vol4\":0,	\"bid5\":0,	\"ask5\":0,	\"bid_vol5\":0,	\"ask_vol5\":0,	\"reversed_bytes0\":0,    \"__comment__3\":\"-价格*100\",	\"reversed_bytes1\":0,    \"__comment__4\":\"市场\",	\"reversed_bytes2\":0,	\"reversed_bytes3\":0,	\"reversed_bytes4\":0,	\"reversed_bytes5\":0,	\"reversed_bytes6\":0,	\"reversed_bytes7\":0,	\"reversed_bytes8\":0,    \"__comment__5\":\"涨速\",	\"reversed_bytes9\":0,    \"__comment__6\":\"活跃度\",	\"active2\":0,	\"lcl_time\":0,    \"lcl_time_str\":\"\",    \"VERSION\":0}");
            nlohmann::json json_var;
            //i >> json_var; //from file
            json_var = nlohmann::json::parse(i);  //from string

            S4::tdx_snap_t tdx_snap_t_var;

            if(!S4::tdx_snap_t::from_json(json_var, tdx_snap_t_var)){
                INFO("S4::tdx_snap_t::from_json fail!");
                return -1;
            }

            nlohmann::json j_out;
            if(!S4::tdx_snap_t::to_json(j_out, tdx_snap_t_var)){
                INFO("S4::tdx_snap_t::to_json fail!");
                return -1;
            }

            INFO("{:}", j_out.dump(4));

            return 0;
        }

        