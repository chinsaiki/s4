/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: .\json_template\gbl_conf_t.json
* Type name: gbl_conf_t
* Tester:    int gbl_conf_t_tester();
*
* Json keep-word: 
    "default_value_fields": [], # Take value in .json file as the default value of cpp variable
    "optional_fields": [], # Not require to present to .json file, but always in cpp struct
    "assign_type_fields": {"field":"cpp-type"}, # Assign specal cpp-type of field, but not infer automatically as default
    "assign_set_lists": [], # Take list in .json file as std::set<>, but not std::vector<> as default
* Script author: ChenZaihui<chinsaiki@outlook.com>
*/
#pragma once
#include <assert.h>
#include "common/s4json_util.h"
#include "common/s4logger.h"

#include <set>
#include <list>
#include <vector>

namespace S4 {

/* type */
struct gbl_conf_t {
	struct logger_t {
		bool enable_console;	//	True
		bool enable_file_all;	//	False
		bool enable_file_all_pure;	//	True
		bool enable_file_err;	//	False
		bool enable_file_err_pure;	//	True
		int level;	//	4
		int max_file_size_MB;	//	9999
		int max_files;	//	10
		std::string save_path;	//	./logs
		std::string file_preamble;	//	S4
	};
	logger_t logger;
	struct db_t {
		std::string root;	//	./db
	};
	db_t db;

	/* from json */
	static bool from_json(const json& json_var, gbl_conf_t& gbl_conf_t_var){
		try{
			const json& json_var_logger = json_var["logger"];
			try{
				gbl_conf_t_var.logger.enable_console = json_var_logger.at("enable_console").get<bool>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "enable_console", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.enable_file_all = json_var_logger.at("enable_file_all").get<bool>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "enable_file_all", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.enable_file_all_pure = json_var_logger.at("enable_file_all_pure").get<bool>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "enable_file_all_pure", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.enable_file_err = json_var_logger.at("enable_file_err").get<bool>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "enable_file_err", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.enable_file_err_pure = json_var_logger.at("enable_file_err_pure").get<bool>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "enable_file_err_pure", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.level = json_var_logger.at("level").get<int>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "level", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.max_file_size_MB = json_var_logger.at("max_file_size_MB").get<int>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "max_file_size_MB", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.max_files = json_var_logger.at("max_files").get<int>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "max_files", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.save_path = json_var_logger.at("save_path").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "save_path", e.what());
				throw e;
			}
			try{
				gbl_conf_t_var.logger.file_preamble = json_var_logger.at("file_preamble").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "file_preamble", e.what());
				throw e;
			}
			const json& json_var_db = json_var["db"];
			try{
				gbl_conf_t_var.db.root = json_var_db.at("root").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "root", e.what());
				throw e;
			}
		}catch (const std::exception& e){
			ERR("parse json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
	/* to json */
	static bool to_json(json& json_var, const gbl_conf_t& gbl_conf_t_var){
		try{
			json json_var_logger;
			json_var_logger["enable_console"] = gbl_conf_t_var.logger.enable_console;
			json_var_logger["enable_file_all"] = gbl_conf_t_var.logger.enable_file_all;
			json_var_logger["enable_file_all_pure"] = gbl_conf_t_var.logger.enable_file_all_pure;
			json_var_logger["enable_file_err"] = gbl_conf_t_var.logger.enable_file_err;
			json_var_logger["enable_file_err_pure"] = gbl_conf_t_var.logger.enable_file_err_pure;
			json_var_logger["level"] = gbl_conf_t_var.logger.level;
			json_var_logger["max_file_size_MB"] = gbl_conf_t_var.logger.max_file_size_MB;
			json_var_logger["max_files"] = gbl_conf_t_var.logger.max_files;
			json_var_logger["save_path"] = gbl_conf_t_var.logger.save_path;
			json_var_logger["file_preamble"] = gbl_conf_t_var.logger.file_preamble;
			json_var["logger"] = json_var_logger;
			json json_var_db;
			json_var_db["root"] = gbl_conf_t_var.db.root;
			json_var["db"] = json_var_db;
		}catch (const std::exception& e){
		ERR("to json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
};// struct gbl_conf_t
} // namespace S4


/* Tester */
inline int gbl_conf_t_tester() {

    //std::ifstream i("E:/work/s4/./json_template/gbl_conf_t.json");
    std::string i("{    \"logger\":{        \"enable_console\" : true,        \"enable_file_all\": false,        \"enable_file_all_pure\":true,        \"enable_file_err\": false,        \"enable_file_err_pure\":true,        \"level\" : 4,        \"max_file_size_MB\" : 9999,        \"max_files\":10,        \"save_path\":\"./logs\",        \"file_preamble\":\"S4\"    },    \"db\":{        \"root\" : \"./db\"    }}");
    S4::json json_var;
    //i >> json_var; //from file
    json_var = S4::json::parse(i);  //from string

    S4::gbl_conf_t gbl_conf_t_var;

    if(!S4::gbl_conf_t::from_json(json_var, gbl_conf_t_var)){
        INFO("S4::gbl_conf_t::from_json fail!");
        return -1;
    }

    S4::json j_out;
    if(!S4::gbl_conf_t::to_json(j_out, gbl_conf_t_var)){
        INFO("S4::gbl_conf_t::to_json fail!");
        return -1;
    }

    INFO("{:}", j_out.dump(4));

    return 0;
}

