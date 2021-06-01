/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: ./json_template\s4_history_trade_t.json
* Type name: s4_history_trade_t
* Tester:    int s4_history_trade_t_tester();
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
struct s4_history_trade_t {
	//id through open-close
	int64_t id;	//	0
	time_date_t date;	//	0
	//strategy name
	std::string stgName;	//	tdx_xyzq
	std::string mktCodeStr;	//	sz000001
	///// name, 平安银行 is not good for sqliteDB
	time_utcSec_t time_utcSec;	//	123
	std::string datetime;	//	2018_04_26__00_00_00
	//current option of id: open / change_take / change_stop / close / change_close / abort
	trade_opt_t optType = trade_opt_t_fromString("oUNKNOWN_OPT");
	//long as stock only for now
	std::string position;	//	long
	//current status of id: new / opened / closed / aborted
	std::string status;	//	new
	price_t order_open;	//	-1
	price_t order_take;	//	-1
	price_t order_stop;	//	-1
	price_t order_close;	//	-1
	vol_share_t order_vol;	//	-1
	price_t deal_open;	//	-1
	price_t deal_close;	//	-1
	//not in use for now
	vol_share_t deal_vol;	//	-1
	amount_t deal_amt;	//	-1.0
	amount_t commission;	//	0.0
	amount_t stamp_duty;	//	0.0
	amount_t transfer_fee;	//	0.0
	amount_t other_fees;	//	0.0
	std::string remarks;	//	起始配号:226168906

	/* from json */
	static bool from_json(const nlohmann::json& json_var, s4_history_trade_t& s4_history_trade_t_var){
		try{
			try{
				s4_history_trade_t_var.id = json_var.at("id").get<int64_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "id", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.date = json_var.at("date").get<time_date_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "date", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.stgName = json_var.at("stgName").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "stgName", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.mktCodeStr = json_var.at("mktCodeStr").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "mktCodeStr", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.time_utcSec = json_var.at("time_utcSec").get<time_utcSec_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "time_utcSec", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.datetime = json_var.at("datetime").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "datetime", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.optType = trade_opt_t_fromString(json_var.at("optType").get<std::string>());
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "optType", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.position = json_var.at("position").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "position", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.status = json_var.at("status").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "status", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.order_open = json_var.at("order_open").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "order_open", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.order_take = json_var.at("order_take").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "order_take", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.order_stop = json_var.at("order_stop").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "order_stop", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.order_close = json_var.at("order_close").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "order_close", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.order_vol = json_var.at("order_vol").get<vol_share_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "order_vol", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.deal_open = json_var.at("deal_open").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "deal_open", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.deal_close = json_var.at("deal_close").get<price_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "deal_close", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.deal_vol = json_var.at("deal_vol").get<vol_share_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "deal_vol", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.deal_amt = json_var.at("deal_amt").get<amount_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "deal_amt", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.commission = json_var.at("commission").get<amount_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "commission", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.stamp_duty = json_var.at("stamp_duty").get<amount_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "stamp_duty", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.transfer_fee = json_var.at("transfer_fee").get<amount_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "transfer_fee", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.other_fees = json_var.at("other_fees").get<amount_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "other_fees", e.what());
				throw e;
			}
			try{
				s4_history_trade_t_var.remarks = json_var.at("remarks").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "remarks", e.what());
				throw e;
			}
		}catch (const std::exception& e){
			ERR("parse json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
	/* to json */
	static bool to_json(nlohmann::json& json_var, const s4_history_trade_t& s4_history_trade_t_var){
		try{
			json_var["id"] = s4_history_trade_t_var.id;
			json_var["date"] = s4_history_trade_t_var.date;
			json_var["stgName"] = s4_history_trade_t_var.stgName;
			json_var["mktCodeStr"] = s4_history_trade_t_var.mktCodeStr;
			json_var["time_utcSec"] = s4_history_trade_t_var.time_utcSec;
			json_var["datetime"] = s4_history_trade_t_var.datetime;
			json_var["optType"] = trade_opt_t_toString(s4_history_trade_t_var.optType);
			json_var["position"] = s4_history_trade_t_var.position;
			json_var["status"] = s4_history_trade_t_var.status;
			json_var["order_open"] = s4_history_trade_t_var.order_open;
			json_var["order_take"] = s4_history_trade_t_var.order_take;
			json_var["order_stop"] = s4_history_trade_t_var.order_stop;
			json_var["order_close"] = s4_history_trade_t_var.order_close;
			json_var["order_vol"] = s4_history_trade_t_var.order_vol;
			json_var["deal_open"] = s4_history_trade_t_var.deal_open;
			json_var["deal_close"] = s4_history_trade_t_var.deal_close;
			json_var["deal_vol"] = s4_history_trade_t_var.deal_vol;
			json_var["deal_amt"] = s4_history_trade_t_var.deal_amt;
			json_var["commission"] = s4_history_trade_t_var.commission;
			json_var["stamp_duty"] = s4_history_trade_t_var.stamp_duty;
			json_var["transfer_fee"] = s4_history_trade_t_var.transfer_fee;
			json_var["other_fees"] = s4_history_trade_t_var.other_fees;
			json_var["remarks"] = s4_history_trade_t_var.remarks;
		}catch (const std::exception& e){
		ERR("to json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}

	bool operator ==(const s4_history_trade_t& d) const
	{
		if (id == d.id &&
			date == d.date &&
			stgName == d.stgName &&
			mktCodeStr == d.mktCodeStr &&
			time_utcSec == d.time_utcSec &&
			datetime == d.datetime &&
			optType == d.optType &&
			position == d.position &&
			status == d.status &&
			order_open == d.order_open &&
			order_take == d.order_take &&
			order_stop == d.order_stop &&
			order_close == d.order_close &&
			order_vol == d.order_vol &&
			deal_open == d.deal_open &&
			deal_close == d.deal_close &&
			deal_vol == d.deal_vol &&
			deal_amt == d.deal_amt &&
			commission == d.commission &&
			stamp_duty == d.stamp_duty &&
			transfer_fee == d.transfer_fee &&
			other_fees == d.other_fees &&
			remarks == d.remarks)
		{
			return true;
		}
		return false;
	}

	bool operator !=(const s4_history_trade_t& d) const
	{
		return !((*this)==d);
	}

};// struct s4_history_trade_t
} // namespace S4


        /* Tester */
        inline int s4_history_trade_t_tester() {

            //std::ifstream i("G:/work2t/99_s3/s4/./json_template/s4_history_trade_t.json");
            std::string i("{    \"__sqlite_capable__\" : true,    \"__sqlite_primary__\" : \"id, time_utcSec, status\",    \"__assign_type_fields__\": {        \"date\":\"time_date_t\",         \"id\":\"int64_t\",         \"time_utcSec\":\"time_utcSec_t\",         \"order_open\":\"price_t\",         \"order_take\":\"price_t\",         \"order_stop\":\"price_t\",         \"order_close\":\"price_t\",         \"order_vol\":\"vol_share_t\",                 \"deal_open\":\"price_t\",         \"deal_close\":\"price_t\",         \"deal_vol\":\"vol_share_t\",         \"deal_amt\":\"amount_t\",         \"commission\":\"amount_t\",         \"stamp_duty\":\"amount_t\",         \"transfer_fee\":\"amount_t\",         \"other_fees\":\"amount_t\"    },    \"__assign_enum_fields__\": {        \"optType\" : \"trade_opt_t\"    },    \"__comment__1\":\"id through open-close\",    \"id\":0,    \"date\":0,    \"__comment__0\":\"strategy name\",    \"stgName\": \"tdx_xyzq\",    \"mktCodeStr\":\"sz000001\",    \"__comment__2\":\"/// name, 平安银行 is not good for sqliteDB\",    \"time_utcSec\": 123,    \"datetime\": \"2018_04_26__00_00_00\",    \"__comment__3\":\"current option of id: open / change_take / change_stop / close / change_close / abort\",    \"optType\":\"oUNKNOWN_OPT\",    \"__comment__4\":\"long as stock only for now\",    \"position\": \"long\",    \"__comment__5\":\"current status of id: new / opened / closed / aborted\",    \"status\":\"new\",    \"order_open\":-1,    \"order_take\":-1,    \"order_stop\":-1,    \"order_close\":-1,    \"order_vol\":-1,    \"deal_open\": -1,     \"deal_close\": -1,         \"__comment__6\":\"not in use for now\",    \"deal_vol\": -1,     \"deal_amt\": -1.0,     \"commission\":0.0,    \"stamp_duty\":0.0,    \"transfer_fee\":0.0,    \"other_fees\":0.0,    \"remarks\":\"起始配号:226168906\"}");
            nlohmann::json json_var;
            //i >> json_var; //from file
            json_var = nlohmann::json::parse(i);  //from string

            S4::s4_history_trade_t s4_history_trade_t_var;

            if(!S4::s4_history_trade_t::from_json(json_var, s4_history_trade_t_var)){
                INFO("S4::s4_history_trade_t::from_json fail!");
                return -1;
            }

            nlohmann::json j_out;
            if(!S4::s4_history_trade_t::to_json(j_out, s4_history_trade_t_var)){
                INFO("S4::s4_history_trade_t::to_json fail!");
                return -1;
            }

            INFO("{:}", j_out.dump(4));

            return 0;
        }

        