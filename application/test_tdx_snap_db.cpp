#include "common/s4conf.h"
#include "db_sqlite/db.h"
#include "jsonTypes/tdx_snap_t_dbTbl.h"
CREATE_LOCAL_LOGGER("test_trans_s3db")

using namespace S4;


int main(int argc, char** argv)
{
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

    std::string db_path("../db/tdx_snap_example_sh600463.db");
	sqlite::DB_t snap_db(db_path);

	std::vector <std::string> dates = snap_db.get_table_list();

    LCL_INFO("tdx_snap {} has {} dates.", db_path, dates.size());
    if (dates.size() == 0){
        LCL_ERR("No snap to read out!");
        return -1;
    }
    LCL_INFO("Going to load snap of {}", dates.back());

    S4::sqlite::tdx_snap_t_dbTbl snap_tbl;
	std::vector<tdx_snap_t> snaps;

	// snap_db.read_table<S4::sqlite::tdx_snap_t_dbTbl::data_t>(&snap_tbl, dates.back(), snaps);
	snap_db.read_table_v2(&snap_tbl, dates.back(), snaps);
    LCL_INFO("{} snaps has been loaded:", snaps.size());

    int nb = 0;
    for (const auto& snap : snaps){
        LCL_INFO("#{} {}", nb, stk_tdx_snap_toString(snap));
        if (nb++ >=10)
            break;
    }

	LCL_INFO("tdx snap db test OK!");

	return 0;
}
