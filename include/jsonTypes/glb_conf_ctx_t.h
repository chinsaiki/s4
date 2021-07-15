/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: ./json_template\glb_conf_ctx_t.json
* Type name: glb_conf_ctx_t
* Tester:    int glb_conf_ctx_t_tester();
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

#include <set>
#include <list>
#include <vector>

namespace S4 {

/* type */
struct glb_conf_ctx_t {
	struct logger_t {
		bool enable_console = (bool)true;
		bool enable_file_all = (bool)false;
		bool enable_file_all_pure = (bool)true;
		bool enable_file_err = (bool)false;
		bool enable_file_err_pure = (bool)true;
		spdlog::level::level_enum level = (spdlog::level::level_enum)2;
		size_t max_file_size_MB = (size_t)9999;
		size_t max_files = (size_t)10;
		std::string save_path = (std::string)"./logs";
		std::string file_preamble = (std::string)"S4";
	};
	logger_t logger;
	struct network_t {
		std::string db_viewer_ip;	//	127.0.0.1
		std::string db_viewer_port;	//	8980
	};
	network_t network;
	struct db_t {
		std::string root;	//	../db
		std::string history_broker;	//	s4_history_tdx_xyzq.db
		std::string history_trade;	//	s4_history_trade.db
		std::string tus_basic;	//	tus_basic.db
		std::string s3_root;	//	../../s3/db
		std::string s3_history;	//	s3orders.db
	};
	db_t db;
	struct tdx_t {
		std::string root;	//	../doc/tdx_test
	};
	tdx_t tdx;
	struct snap_t {
		struct DB_list_t {
			int bgnDate;	//	0
			int endDate;	//	21000000
			std::string path;	//	../db/snap/
		};
		std::vector<glb_conf_ctx_t::snap_t::DB_list_t> DB_list;
	};
	snap_t snap;

	/* from json */
	static bool from_json(const nlohmann::json& json_var, glb_conf_ctx_t& glb_conf_ctx_t_var){
		try{
			const nlohmann::json& json_var_logger = json_var["logger"];
			try{
				glb_conf_ctx_t_var.logger.enable_console = json_var_logger.at("enable_console").get<bool>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.enable_file_all = json_var_logger.at("enable_file_all").get<bool>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.enable_file_all_pure = json_var_logger.at("enable_file_all_pure").get<bool>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.enable_file_err = json_var_logger.at("enable_file_err").get<bool>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.enable_file_err_pure = json_var_logger.at("enable_file_err_pure").get<bool>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.level = json_var_logger.at("level").get<spdlog::level::level_enum>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.max_file_size_MB = json_var_logger.at("max_file_size_MB").get<size_t>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.max_files = json_var_logger.at("max_files").get<size_t>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.save_path = json_var_logger.at("save_path").get<std::string>();
			}catch(...){
			}
			try{
				glb_conf_ctx_t_var.logger.file_preamble = json_var_logger.at("file_preamble").get<std::string>();
			}catch(...){
			}
			const nlohmann::json& json_var_network = json_var["network"];
			try{
				glb_conf_ctx_t_var.network.db_viewer_ip = json_var_network.at("db_viewer_ip").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "db_viewer_ip", e.what());
				throw e;
			}
			try{
				glb_conf_ctx_t_var.network.db_viewer_port = json_var_network.at("db_viewer_port").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "db_viewer_port", e.what());
				throw e;
			}
			const nlohmann::json& json_var_db = json_var["db"];
			try{
				glb_conf_ctx_t_var.db.root = json_var_db.at("root").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "root", e.what());
				throw e;
			}
			try{
				glb_conf_ctx_t_var.db.history_broker = json_var_db.at("history_broker").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "history_broker", e.what());
				throw e;
			}
			try{
				glb_conf_ctx_t_var.db.history_trade = json_var_db.at("history_trade").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "history_trade", e.what());
				throw e;
			}
			try{
				glb_conf_ctx_t_var.db.tus_basic = json_var_db.at("tus_basic").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "tus_basic", e.what());
				throw e;
			}
			try{
				glb_conf_ctx_t_var.db.s3_root = json_var_db.at("s3_root").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "s3_root", e.what());
				throw e;
			}
			try{
				glb_conf_ctx_t_var.db.s3_history = json_var_db.at("s3_history").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "s3_history", e.what());
				throw e;
			}
			const nlohmann::json& json_var_tdx = json_var["tdx"];
			try{
				glb_conf_ctx_t_var.tdx.root = json_var_tdx.at("root").get<std::string>();
			}catch(const std::exception& e){
				ERR("{:} not found in json! e={:}", "root", e.what());
				throw e;
			}
			const nlohmann::json& json_var_snap = json_var["snap"];
			try{
				const nlohmann::json& json_var_snap_DB_list = json_var_snap.at("DB_list");
				for(auto& json_var_snap_DB_list_x: json_var_snap_DB_list.items()){
					glb_conf_ctx_t::snap_t::DB_list_t glb_conf_ctx_t_var_snap_DB_list;
					try{
						glb_conf_ctx_t_var_snap_DB_list.bgnDate = json_var_snap_DB_list_x.value().at("bgnDate").get<int>();
					}catch(const std::exception& e){
						ERR("{:} not found in json! e={:}", "bgnDate", e.what());
						throw e;
					}
					try{
						glb_conf_ctx_t_var_snap_DB_list.endDate = json_var_snap_DB_list_x.value().at("endDate").get<int>();
					}catch(const std::exception& e){
						ERR("{:} not found in json! e={:}", "endDate", e.what());
						throw e;
					}
					try{
						glb_conf_ctx_t_var_snap_DB_list.path = json_var_snap_DB_list_x.value().at("path").get<std::string>();
					}catch(const std::exception& e){
						ERR("{:} not found in json! e={:}", "path", e.what());
						throw e;
					}
					glb_conf_ctx_t_var.snap.DB_list.emplace_back(glb_conf_ctx_t_var_snap_DB_list);
				}
			}catch(const std::exception& e){
				ERR("parse field {:} fail! e={:}", "DB_list", e.what());
				throw e;
			}
		}catch (const std::exception& e){
			ERR("parse json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}
	/* to json */
	static bool to_json(nlohmann::json& json_var, const glb_conf_ctx_t& glb_conf_ctx_t_var){
		try{
			nlohmann::json json_var_logger;
			json_var_logger["enable_console"] = glb_conf_ctx_t_var.logger.enable_console;
			json_var_logger["enable_file_all"] = glb_conf_ctx_t_var.logger.enable_file_all;
			json_var_logger["enable_file_all_pure"] = glb_conf_ctx_t_var.logger.enable_file_all_pure;
			json_var_logger["enable_file_err"] = glb_conf_ctx_t_var.logger.enable_file_err;
			json_var_logger["enable_file_err_pure"] = glb_conf_ctx_t_var.logger.enable_file_err_pure;
			json_var_logger["level"] = glb_conf_ctx_t_var.logger.level;
			json_var_logger["max_file_size_MB"] = glb_conf_ctx_t_var.logger.max_file_size_MB;
			json_var_logger["max_files"] = glb_conf_ctx_t_var.logger.max_files;
			json_var_logger["save_path"] = glb_conf_ctx_t_var.logger.save_path;
			json_var_logger["file_preamble"] = glb_conf_ctx_t_var.logger.file_preamble;
			json_var["logger"] = json_var_logger;
			nlohmann::json json_var_network;
			json_var_network["db_viewer_ip"] = glb_conf_ctx_t_var.network.db_viewer_ip;
			json_var_network["db_viewer_port"] = glb_conf_ctx_t_var.network.db_viewer_port;
			json_var["network"] = json_var_network;
			nlohmann::json json_var_db;
			json_var_db["root"] = glb_conf_ctx_t_var.db.root;
			json_var_db["history_broker"] = glb_conf_ctx_t_var.db.history_broker;
			json_var_db["history_trade"] = glb_conf_ctx_t_var.db.history_trade;
			json_var_db["tus_basic"] = glb_conf_ctx_t_var.db.tus_basic;
			json_var_db["s3_root"] = glb_conf_ctx_t_var.db.s3_root;
			json_var_db["s3_history"] = glb_conf_ctx_t_var.db.s3_history;
			json_var["db"] = json_var_db;
			nlohmann::json json_var_tdx;
			json_var_tdx["root"] = glb_conf_ctx_t_var.tdx.root;
			json_var["tdx"] = json_var_tdx;
			nlohmann::json json_var_snap;
			json_var_snap["DB_list"] = nlohmann::json();
			for(const auto& glb_conf_ctx_t_var_snap_DB_list_x: glb_conf_ctx_t_var.snap.DB_list){
				json json_var_snap_DB_list;
				json_var_snap_DB_list["bgnDate"] = glb_conf_ctx_t_var_snap_DB_list_x.bgnDate;
				json_var_snap_DB_list["endDate"] = glb_conf_ctx_t_var_snap_DB_list_x.endDate;
				json_var_snap_DB_list["path"] = glb_conf_ctx_t_var_snap_DB_list_x.path;
				json_var_snap["DB_list"].push_back(json_var_snap_DB_list);
			}
			json_var["snap"] = json_var_snap;
		}catch (const std::exception& e){
		ERR("to json {:} \nfail:{:}", json_var.dump(4), e.what());
			return false;
		}
		return true;
	}

};// struct glb_conf_ctx_t
} // namespace S4


        /* Tester */
        inline int glb_conf_ctx_t_tester() {

            //std::ifstream i("G:/work2t/99_s3/s4/./json_template/glb_conf_ctx_t.json");
            std::string i("{    \"logger\":{        \"__assign_type_fields__\": {             \"level\" : \"spdlog::level::level_enum\",            \"max_file_size_MB\" : \"size_t\",            \"max_files\" : \"size_t\"        },        \"__default_value_fields__\": [            \"enable_console\",            \"enable_file_all\",            \"enable_file_all_pure\",            \"enable_file_err\",            \"enable_file_err_pure\",            \"level\",            \"max_file_size_MB\",            \"max_files\",            \"save_path\",            \"file_preamble\"        ],        \"enable_console\" : true,        \"enable_file_all\": false,        \"enable_file_all_pure\":true,        \"enable_file_err\": false,        \"enable_file_err_pure\":true,        \"level\" : 2,        \"max_file_size_MB\" : 9999,        \"max_files\":10,        \"save_path\":\"./logs\",        \"file_preamble\":\"S4\"    },    \"network\":{        \"db_viewer_ip\" : \"127.0.0.1\",        \"db_viewer_port\": \"8980\"    },    \"db\":{        \"root\" : \"../db\",        \"history_broker\": \"s4_history_tdx_xyzq.db\",        \"history_trade\": \"s4_history_trade.db\",        \"tus_basic\": \"tus_basic.db\",        \"s3_root\": \"../../s3/db\",        \"s3_history\" : \"s3orders.db\"    },    \"tdx\":{        \"root\" : \"../doc/tdx_test\"    },    \"snap\":{        \"DB_list\":[            {                \"bgnDate\":0,                \"endDate\":21000000,                \"path\":\"../db/snap/\"            }        ]    }}");
            nlohmann::json json_var;
            //i >> json_var; //from file
            json_var = nlohmann::json::parse(i);  //from string

            S4::glb_conf_ctx_t glb_conf_ctx_t_var;

            if(!S4::glb_conf_ctx_t::from_json(json_var, glb_conf_ctx_t_var)){
                INFO("S4::glb_conf_ctx_t::from_json fail!");
                return -1;
            }

            nlohmann::json j_out;
            if(!S4::glb_conf_ctx_t::to_json(j_out, glb_conf_ctx_t_var)){
                INFO("S4::glb_conf_ctx_t::to_json fail!");
                return -1;
            }

            INFO("{:}", j_out.dump(4));

            return 0;
        }

        