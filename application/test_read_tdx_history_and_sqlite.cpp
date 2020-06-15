#include "tdx/read_history.h"
#include "common/s4conf.h"
#include "history/s4_history_data.h"

CREATE_LOCAL_LOGGER("test_tdx_history")

using namespace S4;

int main(int argc, char** argv)
{
	glb_conf::pInstance()->load("../json_template/glb_conf_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

	glb_conf_t::db_t db = glb_conf::pInstance()->db();
	std::filesystem::path db_root_path = db.root;
	std::filesystem::path db_history_qs_path = db_root_path / db.history_tdx_qs;

	sqlite::DB_t qs_history_db(db_history_qs_path.string());
	bool ret;

	//deal <-> DB
	ret = TDX::history_deal_to_DB(qs_history_db, "../doc/20200531��ʷ�ɽ���ѯ.xls", "deal_to20200531");
	if (!ret) {
		LCL_ERR("history_deal_to_DB fail!");
	}

	//order <-> DB
	ret = TDX::history_order_to_DB(qs_history_db, "../doc/20200531��ʷί�в�ѯ.xls", "order_to20200531");
	if (!ret) {
		LCL_ERR("history_order_to_DB fail!");
	}

	std::vector<std::string> tables = qs_history_db.get_table_list();
	for (auto& t : tables) {
		std::map<std::string, std::string> colums = qs_history_db.get_colum_list(t);
		LCL_INFO("{:}", t);
		for (auto& c : colums) {
			LCL_INFO("\t{:}:{:}", c.first, c.second);
		}
	}

	//order/deal DB -> history
	std::vector<s4_history_t> history_data;
	ret = TDX::read_history_DB(db_history_qs_path.string(), history_data);
	if (!ret) {
		LCL_ERR("history_order_to_DB fail!");
	}

	//history -> DB
	std::filesystem::path db_history_path = db_root_path / db.history_order;
	sqlite::DB_t history_db(db_history_path.string());
	ret = history_data_to_DB(history_db, "to20200531", history_data);
	if (!ret) {
		LCL_ERR("history_data_to_DB fail!");
	}
}
