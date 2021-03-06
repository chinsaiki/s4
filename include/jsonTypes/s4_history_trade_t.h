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
    "__cpp_eq_eclude__":[],     # List of variables not involved in equal compare.
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
#include "types/s4convertors.h"

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
			if(json_var.find("id") != json_var.end()){
				try{
					const auto& json_var_id = json_var.at("id");
					if (json_var_id.is_string())
					    s4_history_trade_t_var.id = IntConvertor::convert(json_var_id.get<std::string>());
					else
					    json_var_id.get_to(s4_history_trade_t_var.id);
				}catch(const std::exception& e){
					ERR("Convert \"id\" to \"int64_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"id\" not found in json!");
				return false;
			}
			if(json_var.find("date") != json_var.end()){
				try{
					const auto& json_var_date = json_var.at("date");
					json_var_date.get_to(s4_history_trade_t_var.date);
				}catch(const std::exception& e){
					ERR("Convert \"date\" to \"time_date_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"date\" not found in json!");
				return false;
			}
			if(json_var.find("stgName") != json_var.end()){
				try{
					const auto& json_var_stgName = json_var.at("stgName");
					json_var_stgName.get_to(s4_history_trade_t_var.stgName);
				}catch(const std::exception& e){
					ERR("Convert \"stgName\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"stgName\" not found in json!");
				return false;
			}
			if(json_var.find("mktCodeStr") != json_var.end()){
				try{
					const auto& json_var_mktCodeStr = json_var.at("mktCodeStr");
					json_var_mktCodeStr.get_to(s4_history_trade_t_var.mktCodeStr);
				}catch(const std::exception& e){
					ERR("Convert \"mktCodeStr\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"mktCodeStr\" not found in json!");
				return false;
			}
			if(json_var.find("time_utcSec") != json_var.end()){
				try{
					const auto& json_var_time_utcSec = json_var.at("time_utcSec");
					json_var_time_utcSec.get_to(s4_history_trade_t_var.time_utcSec);
				}catch(const std::exception& e){
					ERR("Convert \"time_utcSec\" to \"time_utcSec_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"time_utcSec\" not found in json!");
				return false;
			}
			if(json_var.find("datetime") != json_var.end()){
				try{
					const auto& json_var_datetime = json_var.at("datetime");
					json_var_datetime.get_to(s4_history_trade_t_var.datetime);
				}catch(const std::exception& e){
					ERR("Convert \"datetime\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"datetime\" not found in json!");
				return false;
			}
			if(json_var.find("optType") != json_var.end()){
				try{
					s4_history_trade_t_var.optType = trade_opt_t_fromString(json_var.at("optType").get<std::string>());
				}catch(const std::exception& e){
					ERR("Convert \"optType\" to \"trade_opt_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"optType\" not found in json!");
				return false;
			}
			if(json_var.find("position") != json_var.end()){
				try{
					const auto& json_var_position = json_var.at("position");
					json_var_position.get_to(s4_history_trade_t_var.position);
				}catch(const std::exception& e){
					ERR("Convert \"position\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"position\" not found in json!");
				return false;
			}
			if(json_var.find("status") != json_var.end()){
				try{
					const auto& json_var_status = json_var.at("status");
					json_var_status.get_to(s4_history_trade_t_var.status);
				}catch(const std::exception& e){
					ERR("Convert \"status\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"status\" not found in json!");
				return false;
			}
			if(json_var.find("order_open") != json_var.end()){
				try{
					const auto& json_var_order_open = json_var.at("order_open");
					json_var_order_open.get_to(s4_history_trade_t_var.order_open);
				}catch(const std::exception& e){
					ERR("Convert \"order_open\" to \"price_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"order_open\" not found in json!");
				return false;
			}
			if(json_var.find("order_take") != json_var.end()){
				try{
					const auto& json_var_order_take = json_var.at("order_take");
					json_var_order_take.get_to(s4_history_trade_t_var.order_take);
				}catch(const std::exception& e){
					ERR("Convert \"order_take\" to \"price_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"order_take\" not found in json!");
				return false;
			}
			if(json_var.find("order_stop") != json_var.end()){
				try{
					const auto& json_var_order_stop = json_var.at("order_stop");
					json_var_order_stop.get_to(s4_history_trade_t_var.order_stop);
				}catch(const std::exception& e){
					ERR("Convert \"order_stop\" to \"price_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"order_stop\" not found in json!");
				return false;
			}
			if(json_var.find("order_close") != json_var.end()){
				try{
					const auto& json_var_order_close = json_var.at("order_close");
					json_var_order_close.get_to(s4_history_trade_t_var.order_close);
				}catch(const std::exception& e){
					ERR("Convert \"order_close\" to \"price_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"order_close\" not found in json!");
				return false;
			}
			if(json_var.find("order_vol") != json_var.end()){
				try{
					const auto& json_var_order_vol = json_var.at("order_vol");
					json_var_order_vol.get_to(s4_history_trade_t_var.order_vol);
				}catch(const std::exception& e){
					ERR("Convert \"order_vol\" to \"vol_share_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"order_vol\" not found in json!");
				return false;
			}
			if(json_var.find("deal_open") != json_var.end()){
				try{
					const auto& json_var_deal_open = json_var.at("deal_open");
					json_var_deal_open.get_to(s4_history_trade_t_var.deal_open);
				}catch(const std::exception& e){
					ERR("Convert \"deal_open\" to \"price_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"deal_open\" not found in json!");
				return false;
			}
			if(json_var.find("deal_close") != json_var.end()){
				try{
					const auto& json_var_deal_close = json_var.at("deal_close");
					json_var_deal_close.get_to(s4_history_trade_t_var.deal_close);
				}catch(const std::exception& e){
					ERR("Convert \"deal_close\" to \"price_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"deal_close\" not found in json!");
				return false;
			}
			if(json_var.find("deal_vol") != json_var.end()){
				try{
					const auto& json_var_deal_vol = json_var.at("deal_vol");
					json_var_deal_vol.get_to(s4_history_trade_t_var.deal_vol);
				}catch(const std::exception& e){
					ERR("Convert \"deal_vol\" to \"vol_share_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"deal_vol\" not found in json!");
				return false;
			}
			if(json_var.find("deal_amt") != json_var.end()){
				try{
					const auto& json_var_deal_amt = json_var.at("deal_amt");
					json_var_deal_amt.get_to(s4_history_trade_t_var.deal_amt);
				}catch(const std::exception& e){
					ERR("Convert \"deal_amt\" to \"amount_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"deal_amt\" not found in json!");
				return false;
			}
			if(json_var.find("commission") != json_var.end()){
				try{
					const auto& json_var_commission = json_var.at("commission");
					json_var_commission.get_to(s4_history_trade_t_var.commission);
				}catch(const std::exception& e){
					ERR("Convert \"commission\" to \"amount_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"commission\" not found in json!");
				return false;
			}
			if(json_var.find("stamp_duty") != json_var.end()){
				try{
					const auto& json_var_stamp_duty = json_var.at("stamp_duty");
					json_var_stamp_duty.get_to(s4_history_trade_t_var.stamp_duty);
				}catch(const std::exception& e){
					ERR("Convert \"stamp_duty\" to \"amount_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"stamp_duty\" not found in json!");
				return false;
			}
			if(json_var.find("transfer_fee") != json_var.end()){
				try{
					const auto& json_var_transfer_fee = json_var.at("transfer_fee");
					json_var_transfer_fee.get_to(s4_history_trade_t_var.transfer_fee);
				}catch(const std::exception& e){
					ERR("Convert \"transfer_fee\" to \"amount_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"transfer_fee\" not found in json!");
				return false;
			}
			if(json_var.find("other_fees") != json_var.end()){
				try{
					const auto& json_var_other_fees = json_var.at("other_fees");
					json_var_other_fees.get_to(s4_history_trade_t_var.other_fees);
				}catch(const std::exception& e){
					ERR("Convert \"other_fees\" to \"amount_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"other_fees\" not found in json!");
				return false;
			}
			if(json_var.find("remarks") != json_var.end()){
				try{
					const auto& json_var_remarks = json_var.at("remarks");
					json_var_remarks.get_to(s4_history_trade_t_var.remarks);
				}catch(const std::exception& e){
					ERR("Convert \"remarks\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"remarks\" not found in json!");
				return false;
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

        