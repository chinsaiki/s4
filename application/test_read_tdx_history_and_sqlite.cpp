#include "tdx/read_history.h"
#include "common/s4conf.h"
#include "trade/s4_history_trade.h"

CREATE_LOCAL_LOGGER("test_tdx_history")

using namespace S4;

int main(int argc, char** argv)
{
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

	glb_conf_ctx_t::db_t db = glb_conf::pInstance()->db();
	std::filesystem::path db_root_path = db.root;
	std::filesystem::path db_history_qs_path = db_root_path / db.history_broker;

	sqlite::DB_t qs_history_db(db_history_qs_path.string());
	bool ret;

	//deal <-> DB
	ret = TDX::history_deal_to_DB(qs_history_db, "../doc/20200531历史成交查询.xls", "deal_to20200531");
	if (!ret) {
		LCL_ERR("history_deal_to_DB fail!");
	}

	//order <-> DB
	ret = TDX::history_order_to_DB(qs_history_db, "../doc/20200531历史委托查询.xls", "order_to20200531");
	if (!ret) {
		LCL_ERR("history_order_to_DB fail!");
	}

	std::set<std::string> tables = qs_history_db.get_table_list();
	for (auto& t : tables) {
		std::vector<std::string> colums = qs_history_db.get_colum_list(t);
		LCL_INFO("{:}", t);
		for (auto& c : colums) {
			LCL_INFO("\t{:}", c);
		}
	}

	//order/deal DB -> history
	std::vector<s4_history_trade_t> history_trade_data;
	ret = TDX::read_history_DB(db_history_qs_path.string(), history_trade_data);
	if (!ret) {
		LCL_ERR("history_order_to_DB fail!");
	}

	//history -> DB
	std::filesystem::path db_history_path = db_root_path / db.history_trade;
	sqlite::DB_t history_db(db_history_path.string());
	ret = history_trade_to_DB(history_trade_data, history_db, "to20200531");
	if (!ret) {
		LCL_ERR("history_trade_to_DB fail!");
	}
}
