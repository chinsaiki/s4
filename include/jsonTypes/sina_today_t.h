/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: ./json_template\sina_today_t.json
* Type name: sina_today_t
* Tester:    int sina_today_t_tester();
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
struct sina_today_t {
	std::string symbol;	//	sh688981
	//代码
	std::string code;	//	688981
	//名称
	std::string name;	//	中芯国际
	//现价
	std::string trade;	//	55.170
	//涨跌额
	double pricechange;	//	-0.83
	//涨跌幅
	double changepercent;	//	-1.482
	//买一
	std::string buy;	//	55.170
	//卖一
	std::string sell;	//	55.180
	//最日收盘价
	std::string settlement;	//	56.000
	//开盘价
	std::string open;	//	56.020
	//最高价
	std::string high;	//	56.150
	//最低价
	std::string low;	//	55.050
	//成交量，股
	int64_t volume;	//	26114656
	//成交额，元
	int64_t amount;	//	1445777487
	//报价时间
	std::string ticktime;	//	15:29:59
	//市盈率
	double per;	//	82.343
	//市净率
	double pb;	//	4.324
	//总市值，万元
	double mktcap;	//	43590427.323642
	//流通市值，万元
	double nmc;	//	6044717.601
	//换手率，成交额/流通市值*100%
	double turnoverratio;	//	2.38348

	/* from json */
	static bool from_json(const nlohmann::json& json_var, sina_today_t& sina_today_t_var){
		try{
			if(json_var.find("symbol") != json_var.end()){
				try{
					const auto& json_var_symbol = json_var.at("symbol");
					json_var_symbol.get_to(sina_today_t_var.symbol);
				}catch(const std::exception& e){
					ERR("Convert \"symbol\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"symbol\" not found in json!");
				return false;
			}
			if(json_var.find("code") != json_var.end()){
				try{
					const auto& json_var_code = json_var.at("code");
					json_var_code.get_to(sina_today_t_var.code);
				}catch(const std::exception& e){
					ERR("Convert \"code\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"code\" not found in json!");
				return false;
			}
			if(json_var.find("name") != json_var.end()){
				try{
					const auto& json_var_name = json_var.at("name");
					json_var_name.get_to(sina_today_t_var.name);
				}catch(const std::exception& e){
					ERR("Convert \"name\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"name\" not found in json!");
				return false;
			}
			if(json_var.find("trade") != json_var.end()){
				try{
					const auto& json_var_trade = json_var.at("trade");
					json_var_trade.get_to(sina_today_t_var.trade);
				}catch(const std::exception& e){
					ERR("Convert \"trade\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"trade\" not found in json!");
				return false;
			}
			if(json_var.find("pricechange") != json_var.end()){
				try{
					const auto& json_var_pricechange = json_var.at("pricechange");
					if (json_var_pricechange.is_string())
					    sina_today_t_var.pricechange = DoubleConvertor::convert(json_var_pricechange.get<std::string>());
					else
					    json_var_pricechange.get_to(sina_today_t_var.pricechange);
				}catch(const std::exception& e){
					ERR("Convert \"pricechange\" to \"double\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"pricechange\" not found in json!");
				return false;
			}
			if(json_var.find("changepercent") != json_var.end()){
				try{
					const auto& json_var_changepercent = json_var.at("changepercent");
					if (json_var_changepercent.is_string())
					    sina_today_t_var.changepercent = DoubleConvertor::convert(json_var_changepercent.get<std::string>());
					else
					    json_var_changepercent.get_to(sina_today_t_var.changepercent);
				}catch(const std::exception& e){
					ERR("Convert \"changepercent\" to \"double\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"changepercent\" not found in json!");
				return false;
			}
			if(json_var.find("buy") != json_var.end()){
				try{
					const auto& json_var_buy = json_var.at("buy");
					json_var_buy.get_to(sina_today_t_var.buy);
				}catch(const std::exception& e){
					ERR("Convert \"buy\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"buy\" not found in json!");
				return false;
			}
			if(json_var.find("sell") != json_var.end()){
				try{
					const auto& json_var_sell = json_var.at("sell");
					json_var_sell.get_to(sina_today_t_var.sell);
				}catch(const std::exception& e){
					ERR("Convert \"sell\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"sell\" not found in json!");
				return false;
			}
			if(json_var.find("settlement") != json_var.end()){
				try{
					const auto& json_var_settlement = json_var.at("settlement");
					json_var_settlement.get_to(sina_today_t_var.settlement);
				}catch(const std::exception& e){
					ERR("Convert \"settlement\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"settlement\" not found in json!");
				return false;
			}
			if(json_var.find("open") != json_var.end()){
				try{
					const auto& json_var_open = json_var.at("open");
					json_var_open.get_to(sina_today_t_var.open);
				}catch(const std::exception& e){
					ERR("Convert \"open\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"open\" not found in json!");
				return false;
			}
			if(json_var.find("high") != json_var.end()){
				try{
					const auto& json_var_high = json_var.at("high");
					json_var_high.get_to(sina_today_t_var.high);
				}catch(const std::exception& e){
					ERR("Convert \"high\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"high\" not found in json!");
				return false;
			}
			if(json_var.find("low") != json_var.end()){
				try{
					const auto& json_var_low = json_var.at("low");
					json_var_low.get_to(sina_today_t_var.low);
				}catch(const std::exception& e){
					ERR("Convert \"low\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"low\" not found in json!");
				return false;
			}
			if(json_var.find("volume") != json_var.end()){
				try{
					const auto& json_var_volume = json_var.at("volume");
					if (json_var_volume.is_string())
					    sina_today_t_var.volume = IntConvertor::convert(json_var_volume.get<std::string>());
					else
					    json_var_volume.get_to(sina_today_t_var.volume);
				}catch(const std::exception& e){
					ERR("Convert \"volume\" to \"int64_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"volume\" not found in json!");
				return false;
			}
			if(json_var.find("amount") != json_var.end()){
				try{
					const auto& json_var_amount = json_var.at("amount");
					if (json_var_amount.is_string())
					    sina_today_t_var.amount = IntConvertor::convert(json_var_amount.get<std::string>());
					else
					    json_var_amount.get_to(sina_today_t_var.amount);
				}catch(const std::exception& e){
					ERR("Convert \"amount\" to \"int64_t\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"amount\" not found in json!");
				return false;
			}
			if(json_var.find("ticktime") != json_var.end()){
				try{
					const auto& json_var_ticktime = json_var.at("ticktime");
					json_var_ticktime.get_to(sina_today_t_var.ticktime);
				}catch(const std::exception& e){
					ERR("Convert \"ticktime\" to \"std::string\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"ticktime\" not found in json!");
				return false;
			}
			if(json_var.find("per") != json_var.end()){
				try{
					const auto& json_var_per = json_var.at("per");
					if (json_var_per.is_string())
					    sina_today_t_var.per = DoubleConvertor::convert(json_var_per.get<std::string>());
					else
					    json_var_per.get_to(sina_today_t_var.per);
				}catch(const std::exception& e){
					ERR("Convert \"per\" to \"double\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"per\" not found in json!");
				return false;
			}
			if(json_var.find("pb") != json_var.end()){
				try{
					const auto& json_var_pb = json_var.at("pb");
					if (json_var_pb.is_string())
					    sina_today_t_var.pb = DoubleConvertor::convert(json_var_pb.get<std::string>());
					else
					    json_var_pb.get_to(sina_today_t_var.pb);
				}catch(const std::exception& e){
					ERR("Convert \"pb\" to \"double\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"pb\" not found in json!");
				return false;
			}
			if(json_var.find("mktcap") != json_var.end()){
				try{
					const auto& json_var_mktcap = json_var.at("mktcap");
					if (json_var_mktcap.is_string())
					    sina_today_t_var.mktcap = DoubleConvertor::convert(json_var_mktcap.get<std::string>());
					else
					    json_var_mktcap.get_to(sina_today_t_var.mktcap);
				}catch(const std::exception& e){
					ERR("Convert \"mktcap\" to \"double\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"mktcap\" not found in json!");
				return false;
			}
			if(json_var.find("nmc") != json_var.end()){
				try{
					const auto& json_var_nmc = json_var.at("nmc");
					if (json_var_nmc.is_string())
					    sina_today_t_var.nmc = DoubleConvertor::convert(json_var_nmc.get<std::string>());
					else
					    json_var_nmc.get_to(sina_today_t_var.nmc);
				}catch(const std::exception& e){
					ERR("Convert \"nmc\" to \"double\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"nmc\" not found in json!");
				return false;
			}
			if(json_var.find("turnoverratio") != json_var.end()){
				try{
					const auto& json_var_turnoverratio = json_var.at("turnoverratio");
					if (json_var_turnoverratio.is_string())
					    sina_today_t_var.turnoverratio = DoubleConvertor::convert(json_var_turnoverratio.get<std::string>());
					else
					    json_var_turnoverratio.get_to(sina_today_t_var.turnoverratio);
				}catch(const std::exception& e){
					ERR("Convert \"turnoverratio\" to \"double\" fail! e={:}", e.what());
					throw e;
				}
			}else{
				ERR("\"turnoverratio\" not found in json!");
				return false;
			}
		}catch (const std::exception& e){
			ERR("parse json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
	/* to json */
	static bool to_json(nlohmann::json& json_var, const sina_today_t& sina_today_t_var){
		try{
			json_var["symbol"] = sina_today_t_var.symbol;
			json_var["code"] = sina_today_t_var.code;
			json_var["name"] = sina_today_t_var.name;
			json_var["trade"] = sina_today_t_var.trade;
			json_var["pricechange"] = sina_today_t_var.pricechange;
			json_var["changepercent"] = sina_today_t_var.changepercent;
			json_var["buy"] = sina_today_t_var.buy;
			json_var["sell"] = sina_today_t_var.sell;
			json_var["settlement"] = sina_today_t_var.settlement;
			json_var["open"] = sina_today_t_var.open;
			json_var["high"] = sina_today_t_var.high;
			json_var["low"] = sina_today_t_var.low;
			json_var["volume"] = sina_today_t_var.volume;
			json_var["amount"] = sina_today_t_var.amount;
			json_var["ticktime"] = sina_today_t_var.ticktime;
			json_var["per"] = sina_today_t_var.per;
			json_var["pb"] = sina_today_t_var.pb;
			json_var["mktcap"] = sina_today_t_var.mktcap;
			json_var["nmc"] = sina_today_t_var.nmc;
			json_var["turnoverratio"] = sina_today_t_var.turnoverratio;
		}catch (const std::exception& e){
		ERR("to json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}

	bool operator ==(const sina_today_t& d) const
	{
		if (symbol == d.symbol &&
			code == d.code &&
			name == d.name &&
			trade == d.trade &&
			pricechange == d.pricechange &&
			changepercent == d.changepercent &&
			buy == d.buy &&
			sell == d.sell &&
			settlement == d.settlement &&
			open == d.open &&
			high == d.high &&
			low == d.low &&
			volume == d.volume &&
			amount == d.amount &&
			ticktime == d.ticktime &&
			per == d.per &&
			pb == d.pb &&
			mktcap == d.mktcap &&
			nmc == d.nmc &&
			turnoverratio == d.turnoverratio)
		{
			return true;
		}
		return false;
	}

	bool operator !=(const sina_today_t& d) const
	{
		return !((*this)==d);
	}

};// struct sina_today_t
} // namespace S4


        /* Tester */
        inline int sina_today_t_tester() {

            //std::ifstream i("G:/work2t/99_s3/s4/./json_template/sina_today_t.json");
            std::string i("{    \"__assign_type_fields__\": {        \"volume\":\"int64_t\",         \"amount\":\"int64_t\"    },    \"symbol\": \"sh688981\",    \"__comment__0\":\"代码\",    \"code\": \"688981\",    \"__comment__1\":\"名称\",    \"name\": \"\u4e2d\u82af\u56fd\u9645\",    \"__comment__2\":\"现价\",    \"trade\": \"55.170\",    \"__comment__3\":\"涨跌额\",    \"pricechange\": -0.83,    \"__comment__4\":\"涨跌幅\",    \"changepercent\": -1.482,    \"__comment__5\":\"买一\",    \"buy\": \"55.170\",    \"__comment__6\":\"卖一\",    \"sell\": \"55.180\",    \"__comment__7\":\"最日收盘价\",    \"settlement\": \"56.000\",    \"__comment__8\":\"开盘价\",    \"open\": \"56.020\",    \"__comment__9\":\"最高价\",    \"high\": \"56.150\",    \"__comment__a\":\"最低价\",    \"low\": \"55.050\",    \"__comment__b\":\"成交量，股\",    \"volume\": 26114656,    \"__comment__c\":\"成交额，元\",    \"amount\": 1445777487,    \"__comment__d\":\"报价时间\",    \"ticktime\": \"15:29:59\",    \"__comment__e\":\"市盈率\",    \"per\": 82.343,    \"__comment__f\":\"市净率\",    \"pb\": 4.324,    \"__comment__g\":\"总市值，万元\",    \"mktcap\": 43590427.323642,    \"__comment__h\":\"流通市值，万元\",    \"nmc\": 6044717.601,    \"__comment__\":\"换手率，成交额/流通市值*100%\",    \"turnoverratio\": 2.38348}");
            nlohmann::json json_var;
            //i >> json_var; //from file
            json_var = nlohmann::json::parse(i);  //from string

            S4::sina_today_t sina_today_t_var;

            if(!S4::sina_today_t::from_json(json_var, sina_today_t_var)){
                INFO("S4::sina_today_t::from_json fail!");
                return -1;
            }

            nlohmann::json j_out;
            if(!S4::sina_today_t::to_json(j_out, sina_today_t_var)){
                INFO("S4::sina_today_t::to_json fail!");
                return -1;
            }

            INFO("{:}", j_out.dump(4));

            return 0;
        }

        