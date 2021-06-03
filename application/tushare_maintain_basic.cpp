#include "common/s4conf.h"
#include "tus/basic.h"

CREATE_LOCAL_LOGGER("tushare_maintain_basic")

using namespace S4;
using namespace std;


int main(int argc, char** argv)
{
    if (argc == 1){
	    glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
    }else{
        glb_conf::pInstance()->load(argv[1]);
    }
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

    std::shared_ptr<tdxCalendar_t> calendar = glb_conf::pInstance()->calendar();

    //if (S4::TUS::download_basic_to_db(std::to_string(calendar->newest()), true, true)){
    //    LCL_INFO("OK!");
    //}else{
    //    LCL_FATAL("NG!");
    //}

	glb_conf_ctx_t::db_t db = glb_conf::pInstance()->db();
	std::filesystem::path db_root_path = db.root;
	std::filesystem::path db_tus_basic_path = db_root_path / db.tus_basic;
	sqlite::DB_t basic_db(db_tus_basic_path.string());
    std::set<std::string> basic_db_tables = basic_db.get_table_list();

    rptTime rpt("download all basic");
    size_t all = calendar->size();
    size_t now = 0;

    int i = -1;
    do {
        int date = calendar->getDate(i--);
        if (date == 0)
            break;
        
        rpt.estimate(now++, all);

        std::string tu_table = "tus_" + to_string(date);

        bool existing = false;
        for (auto& t : basic_db_tables) {
            if (t == tu_table) {
                existing = true;
                break;
            }
        }
        if (existing)
            continue;
        if (!TUS::download_basic_to_db(std::to_string(date), false, true)) {
            LCL_ERR("{:} fail!", date);
        }
    } while (true);

    return 0;
}