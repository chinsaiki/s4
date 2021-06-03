#include "common/s4conf.h"
#include "trade/s3_history.h"
#include "trade//s4_history_trade.h"
CREATE_LOCAL_LOGGER("test_trans_s3db")

using namespace S4;


int main(int argc, char** argv)
{
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

	bool ret;

	glb_conf_ctx_t::db_t db = glb_conf::pInstance()->db();
	std::filesystem::path db_s3_root_path = db.s3_root;
	std::filesystem::path db_s3_history_path = db_s3_root_path / db.s3_history;
	sqlite::DB_t s3_history_db(db_s3_history_path.string());

	std::set<std::string> s3_tbls = s3_history_db.get_table_list();

	std::vector<s3_order_t> s3data;
	std::vector<s4_history_trade_t> s4data;

	std::string tbl_name;
	if (argc >= 2) {
		tbl_name = argv[1];
	}
	else {
		LCL_FATAL("usage: {:} <tbl_name>", argv[1]);
	}

	std::string comment = "";
	if (argc >= 3) {
		comment = argv[2];
	}

	LCL_INFO("table name = {:}", tbl_name);

	ret = S3c::read_s3history(s3_history_db, tbl_name, "", s3data);

	ret = S3c::trans_s3history(s3data, s4data);

	std::filesystem::path db_s4_root_path = db.root;
	std::filesystem::path db_s4_history_path = db_s4_root_path / db.history_trade;
	sqlite::DB_t s4_history_db(db_s4_history_path.string());
	ret = history_trade_to_DB(s4data, s4_history_db, "s3_" + tbl_name + "_" + comment);

	LCL_INFO("s3 db -> s4 : done! {}", ret);

	return 0;
}
