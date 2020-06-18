/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: ./json_template\tdx_xyzq_history_order_t.json
* Type name: tdx_xyzq_history_order_t
* Tester:    int tdx_xyzq_history_order_t_tester();
*
* Json keep-word: 
    "__default_value_fields__": [], # Take value in .json file as the default value of cpp variable
    "__optional_fields__": [], # Not require to present to .json file, but always in cpp struct
    "__assign_type_fields__": {"field":"cpp-type"}, # Assign specal cpp-type of field, but not infer automatically as default
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
struct tdx_xyzq_history_order_t {
	time_date_t date;	//	20200507
	std::string time_format;	//	19:55:30
	time_utcSec_t time_utcSec;	//	123
	std::string stock_code;	//	002988
	std::string stock_name;	//	豪美新材
	int64_t id;	//	26
	std::string opt_type;	//	买入
	std::string delegate_type;	//	信用交易/撤单
	std::string status;	//	已报
	fprice_t order_price;	//	3.94
	vol_share_t order_vol;	//	5600
	fprice_t deal_price;	//	0.0
	vol_share_t deal_vol;	//	0
	std::string quote_mode;	//	买卖

	/* from json */
	static bool from_json(const nlohmann::json& json_var, tdx_xyzq_history_order_t& tdx_xyzq_history_order_t_var){
		try{
			try{
				tdx_xyzq_history_order_t_var.date = json_var.at("date").get<time_date_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "date", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.time_format = json_var.at("time_format").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "time_format", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.time_utcSec = json_var.at("time_utcSec").get<time_utcSec_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "time_utcSec", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.stock_code = json_var.at("stock_code").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "stock_code", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.stock_name = json_var.at("stock_name").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "stock_name", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.id = json_var.at("id").get<int64_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "id", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.opt_type = json_var.at("opt_type").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "opt_type", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.delegate_type = json_var.at("delegate_type").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "delegate_type", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.status = json_var.at("status").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "status", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.order_price = json_var.at("order_price").get<fprice_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "order_price", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.order_vol = json_var.at("order_vol").get<vol_share_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "order_vol", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.deal_price = json_var.at("deal_price").get<fprice_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "deal_price", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.deal_vol = json_var.at("deal_vol").get<vol_share_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "deal_vol", e.what());
				throw e;
			}
			try{
				tdx_xyzq_history_order_t_var.quote_mode = json_var.at("quote_mode").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "quote_mode", e.what());
				throw e;
			}
		}catch (const std::exception& e){
			ERR("parse json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
	/* to json */
	static bool to_json(nlohmann::json& json_var, const tdx_xyzq_history_order_t& tdx_xyzq_history_order_t_var){
		try{
			json_var["date"] = tdx_xyzq_history_order_t_var.date;
			json_var["time_format"] = tdx_xyzq_history_order_t_var.time_format;
			json_var["time_utcSec"] = tdx_xyzq_history_order_t_var.time_utcSec;
			json_var["stock_code"] = tdx_xyzq_history_order_t_var.stock_code;
			json_var["stock_name"] = tdx_xyzq_history_order_t_var.stock_name;
			json_var["id"] = tdx_xyzq_history_order_t_var.id;
			json_var["opt_type"] = tdx_xyzq_history_order_t_var.opt_type;
			json_var["delegate_type"] = tdx_xyzq_history_order_t_var.delegate_type;
			json_var["status"] = tdx_xyzq_history_order_t_var.status;
			json_var["order_price"] = tdx_xyzq_history_order_t_var.order_price;
			json_var["order_vol"] = tdx_xyzq_history_order_t_var.order_vol;
			json_var["deal_price"] = tdx_xyzq_history_order_t_var.deal_price;
			json_var["deal_vol"] = tdx_xyzq_history_order_t_var.deal_vol;
			json_var["quote_mode"] = tdx_xyzq_history_order_t_var.quote_mode;
		}catch (const std::exception& e){
		ERR("to json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}

	bool operator ==(const tdx_xyzq_history_order_t& d) const
	{
		if (date == d.date &&
			time_format == d.time_format &&
			time_utcSec == d.time_utcSec &&
			stock_code == d.stock_code &&
			stock_name == d.stock_name &&
			id == d.id &&
			opt_type == d.opt_type &&
			delegate_type == d.delegate_type &&
			status == d.status &&
			order_price == d.order_price &&
			order_vol == d.order_vol &&
			deal_price == d.deal_price &&
			deal_vol == d.deal_vol &&
			quote_mode == d.quote_mode)
		{
			return true;
		}
		return false;
	}

	bool operator !=(const tdx_xyzq_history_order_t& d) const
	{
		return !((*this)==d);
	}

};// struct tdx_xyzq_history_order_t
} // namespace S4


        /* Tester */
        inline int tdx_xyzq_history_order_t_tester() {

            //std::ifstream i("G:/E/work/999_s/s4/./json_template/tdx_xyzq_history_order_t.json");
            std::string i("{    \"__assign_type_fields__\": {        \"date\":\"time_date_t\",         \"id\":\"int64_t\",         \"time_utcSec\":\"time_utcSec_t\",         \"deal_price\":\"fprice_t\",         \"order_price\":\"fprice_t\",         \"order_vol\":\"vol_share_t\",         \"deal_vol\":\"vol_share_t\"    },    \"__sqlite_capable__\" : true,    \"date\" : 20200507,    \"time_format\": \"19:55:30\",    \"time_utcSec\": 123,    \"stock_code\": \"002988\",    \"stock_name\": \"豪美新材\",    \"id\":26,    \"opt_type\":\"买入\",    \"delegate_type\":\"信用交易/撤单\",    \"status\":\"已报\",    \"order_price\":3.94,    \"order_vol\":5600,    \"deal_price\":0.0,    \"deal_vol\":0,    \"quote_mode\":\"买卖\"}");
            nlohmann::json json_var;
            //i >> json_var; //from file
            json_var = nlohmann::json::parse(i);  //from string

            S4::tdx_xyzq_history_order_t tdx_xyzq_history_order_t_var;

            if(!S4::tdx_xyzq_history_order_t::from_json(json_var, tdx_xyzq_history_order_t_var)){
                INFO("S4::tdx_xyzq_history_order_t::from_json fail!");
                return -1;
            }

            nlohmann::json j_out;
            if(!S4::tdx_xyzq_history_order_t::to_json(j_out, tdx_xyzq_history_order_t_var)){
                INFO("S4::tdx_xyzq_history_order_t::to_json fail!");
                return -1;
            }

            INFO("{:}", j_out.dump(4));

            return 0;
        }

        