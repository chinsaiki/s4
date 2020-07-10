/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: ./json_template\tushare_basic_t.json
* Type name: tushare_basic_t
* Tester:    int tushare_basic_t_tester();
*
* Json keep-word: 
    "__default_value_fields__": [], # Take value in .json file as the default value of cpp variable
    "__optional_fields__": [], # Not require to present to .json file, but always in cpp struct
    "__assign_type_fields__": {"field":"cpp-type"}, # Assign specal cpp-type of field, but not infer automatically as default.
    "__assign_enum_fields__": {"field":"enum-type"}, # Assign specal enum-type of field, but not infer automatically as default.
                              enum-type need have implemented _toSting() & _fromString() functions.
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
struct tushare_basic_t {
	pureCodeI_t code;	//	600001
	std::string name;	//	
	std::string industry;	//	
	std::string area;	//	
	double pe;	//	0.0
	double outstanding;	//	0.0
	double totals;	//	0.0
	double totalAssets;	//	0.0
	double liquidAssets;	//	0.0
	double fixedAssets;	//	0.0
	double reserved;	//	0.0
	double reservedPerShare;	//	0.0
	double esp;	//	0.0
	double bvps;	//	0.0
	double pb;	//	0.0
	time_date_t timeToMarket;	//	0
	double undp;	//	0.0
	double perundp;	//	0.0
	double rev;	//	0.0
	double profit;	//	0.0
	double gpr;	//	0.0
	double npr;	//	0.0
	int holders;	//	0

	/* from json */
	static bool from_json(const nlohmann::json& json_var, tushare_basic_t& tushare_basic_t_var){
		try{
			try{
				tushare_basic_t_var.code = json_var.at("code").get<pureCodeI_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "code", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.name = json_var.at("name").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "name", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.industry = json_var.at("industry").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "industry", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.area = json_var.at("area").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "area", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.pe = json_var.at("pe").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "pe", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.outstanding = json_var.at("outstanding").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "outstanding", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.totals = json_var.at("totals").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "totals", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.totalAssets = json_var.at("totalAssets").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "totalAssets", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.liquidAssets = json_var.at("liquidAssets").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "liquidAssets", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.fixedAssets = json_var.at("fixedAssets").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "fixedAssets", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.reserved = json_var.at("reserved").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reserved", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.reservedPerShare = json_var.at("reservedPerShare").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "reservedPerShare", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.esp = json_var.at("esp").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "esp", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.bvps = json_var.at("bvps").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "bvps", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.pb = json_var.at("pb").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "pb", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.timeToMarket = json_var.at("timeToMarket").get<time_date_t>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "timeToMarket", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.undp = json_var.at("undp").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "undp", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.perundp = json_var.at("perundp").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "perundp", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.rev = json_var.at("rev").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "rev", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.profit = json_var.at("profit").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "profit", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.gpr = json_var.at("gpr").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "gpr", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.npr = json_var.at("npr").get<double>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "npr", e.what());
				throw e;
			}
			try{
				tushare_basic_t_var.holders = json_var.at("holders").get<int>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "holders", e.what());
				throw e;
			}
		}catch (const std::exception& e){
			ERR("parse json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
	/* to json */
	static bool to_json(nlohmann::json& json_var, const tushare_basic_t& tushare_basic_t_var){
		try{
			json_var["code"] = tushare_basic_t_var.code;
			json_var["name"] = tushare_basic_t_var.name;
			json_var["industry"] = tushare_basic_t_var.industry;
			json_var["area"] = tushare_basic_t_var.area;
			json_var["pe"] = tushare_basic_t_var.pe;
			json_var["outstanding"] = tushare_basic_t_var.outstanding;
			json_var["totals"] = tushare_basic_t_var.totals;
			json_var["totalAssets"] = tushare_basic_t_var.totalAssets;
			json_var["liquidAssets"] = tushare_basic_t_var.liquidAssets;
			json_var["fixedAssets"] = tushare_basic_t_var.fixedAssets;
			json_var["reserved"] = tushare_basic_t_var.reserved;
			json_var["reservedPerShare"] = tushare_basic_t_var.reservedPerShare;
			json_var["esp"] = tushare_basic_t_var.esp;
			json_var["bvps"] = tushare_basic_t_var.bvps;
			json_var["pb"] = tushare_basic_t_var.pb;
			json_var["timeToMarket"] = tushare_basic_t_var.timeToMarket;
			json_var["undp"] = tushare_basic_t_var.undp;
			json_var["perundp"] = tushare_basic_t_var.perundp;
			json_var["rev"] = tushare_basic_t_var.rev;
			json_var["profit"] = tushare_basic_t_var.profit;
			json_var["gpr"] = tushare_basic_t_var.gpr;
			json_var["npr"] = tushare_basic_t_var.npr;
			json_var["holders"] = tushare_basic_t_var.holders;
		}catch (const std::exception& e){
		ERR("to json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}

	bool operator ==(const tushare_basic_t& d) const
	{
		if (code == d.code &&
			name == d.name &&
			industry == d.industry &&
			area == d.area &&
			pe == d.pe &&
			outstanding == d.outstanding &&
			totals == d.totals &&
			totalAssets == d.totalAssets &&
			liquidAssets == d.liquidAssets &&
			fixedAssets == d.fixedAssets &&
			reserved == d.reserved &&
			reservedPerShare == d.reservedPerShare &&
			esp == d.esp &&
			bvps == d.bvps &&
			pb == d.pb &&
			timeToMarket == d.timeToMarket &&
			undp == d.undp &&
			perundp == d.perundp &&
			rev == d.rev &&
			profit == d.profit &&
			gpr == d.gpr &&
			npr == d.npr &&
			holders == d.holders)
		{
			return true;
		}
		return false;
	}

	bool operator !=(const tushare_basic_t& d) const
	{
		return !((*this)==d);
	}

};// struct tushare_basic_t
} // namespace S4


        /* Tester */
        inline int tushare_basic_t_tester() {

            //std::ifstream i("G:/E/work/999_s/s4/./json_template/tushare_basic_t.json");
            std::string i("{    \"__assign_type_fields__\": {        \"code\": \"pureCodeI_t\",        \"timeToMarket\":\"time_date_t\"    },    \"__sqlite_capable__\" : true,    \"__sqlite_primary__\" : \"code\",    \"code\": 600001,    \"name\": \"\",    \"industry\": \"\",    \"area\": \"\",    \"pe\": 0.0,    \"outstanding\": 0.0,    \"totals\": 0.0,    \"totalAssets\": 0.0,    \"liquidAssets\": 0.0,    \"fixedAssets\": 0.0,    \"reserved\": 0.0,    \"reservedPerShare\": 0.0,    \"esp\": 0.0,    \"bvps\": 0.0,    \"pb\": 0.0,    \"timeToMarket\": 0,    \"undp\": 0.0,    \"perundp\": 0.0,    \"rev\": 0.0,    \"profit\": 0.0,    \"gpr\": 0.0,    \"npr\": 0.0,    \"holders\": 0}");
            nlohmann::json json_var;
            //i >> json_var; //from file
            json_var = nlohmann::json::parse(i);  //from string

            S4::tushare_basic_t tushare_basic_t_var;

            if(!S4::tushare_basic_t::from_json(json_var, tushare_basic_t_var)){
                INFO("S4::tushare_basic_t::from_json fail!");
                return -1;
            }

            nlohmann::json j_out;
            if(!S4::tushare_basic_t::to_json(j_out, tushare_basic_t_var)){
                INFO("S4::tushare_basic_t::to_json fail!");
                return -1;
            }

            INFO("{:}", j_out.dump(4));

            return 0;
        }

        