/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: ./json_template\ifeng_history_daily_t.json
* Type name: ifeng_history_daily_t
* Tester:    int ifeng_history_daily_t_tester();
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
struct ifeng_history_daily_t {
	std::vector<std::vector<std::string>> record;	//	['2019-01-14', '2.550', '2.560', '2.530', '2.520', '123540.00', '-0.020', '-0.78', '2.530', '2.530', '2.530', '123,540.00', '123,540.00', '123,540.00', '0.63']

	/* from json */
	static bool from_json(const nlohmann::json& json_var, ifeng_history_daily_t& ifeng_history_daily_t_var){
		try{
			try{
				ifeng_history_daily_t_var.record = json_var.at("record").get<std::vector<std::vector<std::string>>>();
			}catch(...){
			}
		}catch (const std::exception& e){
			ERR("parse json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
	/* to json */
	static bool to_json(nlohmann::json& json_var, const ifeng_history_daily_t& ifeng_history_daily_t_var){
		try{
			json_var["record"] = ifeng_history_daily_t_var.record;
		}catch (const std::exception& e){
		ERR("to json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}

};// struct ifeng_history_daily_t
} // namespace S4


        /* Tester */
        inline int ifeng_history_daily_t_tester() {

            //std::ifstream i("G:/work2t/99_s3/s4/./json_template/ifeng_history_daily_t.json");
            std::string i("{    \"__assign_type_fields__\":{        \"record\":\"std::vector<std::vector<std::string>>\"    },    \"record\": [        [            \"2019-01-14\",            \"2.550\",            \"2.560\",            \"2.530\",            \"2.520\",            \"123540.00\",            \"-0.020\",            \"-0.78\",            \"2.530\",            \"2.530\",            \"2.530\",            \"123,540.00\",            \"123,540.00\",            \"123,540.00\",            \"0.63\"        ],        [            \"2019-01-15\",            \"2.530\",            \"2.550\",            \"2.540\",            \"2.500\",            \"143025.91\",            \"0.010\",            \"0.40\",            \"2.535\",            \"2.535\",            \"2.535\",            \"133,282.96\",            \"133,282.96\",            \"133,282.96\",            \"0.73\"        ]    ]}");
            nlohmann::json json_var;
            //i >> json_var; //from file
            json_var = nlohmann::json::parse(i);  //from string

            S4::ifeng_history_daily_t ifeng_history_daily_t_var;

            if(!S4::ifeng_history_daily_t::from_json(json_var, ifeng_history_daily_t_var)){
                INFO("S4::ifeng_history_daily_t::from_json fail!");
                return -1;
            }

            nlohmann::json j_out;
            if(!S4::ifeng_history_daily_t::to_json(j_out, ifeng_history_daily_t_var)){
                INFO("S4::ifeng_history_daily_t::to_json fail!");
                return -1;
            }

            INFO("{:}", j_out.dump(4));

            return 0;
        }

        