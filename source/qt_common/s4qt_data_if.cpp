#include "qt_common/s4qt_data_if.h"
#include "common/s4logger.h"

CREATE_LOCAL_LOGGER("qt_data_if")
#include "trade/s4_history_trade.h"
#include "jsonTypes/s4_history_trade_t_dbTbl.h"

namespace S4 {
namespace QT{

void s4qt_data_if::getInfo(const std::string & stkName, const struct S4::stkInfoReq_t& infoReq, class S4::stkInfo_t*& info)
{
	if (_th_data_if.getNowLib() == nullptr || _th_data_if.getNowLib()->count(stkName) == 0) {
		_th_data_if.preload({ stkName }, infoReq);
		while (!_th_data_if.preloadReady())
		{
			process_sleep(0.01);
		}
		_th_data_if.usePreload(true);
	}

	if (_th_data_if.getNowLib()->count(stkName) == 0) {
		info = nullptr;
		LCL_WARN("no such instrument: {:}", stkName);
		return;
	}

	info = _th_data_if.getNowLib()->get(stkName);
	LCL_INFO("getInfo {:} successd: {:} ~ {:} = {:}", stkName, info->pDayKQ->front()->_date, info->pDayKQ->back()->_date, info->pDayKQ->size());
	info->newAtBack();
}

void s4qt_data_if::loadOrdres(const std::string & stkName, const std::string & stgName, const std::string & table_name, std::vector<s4_history_trade_t>& history_trade_data)
{

	glb_conf_ctx_t::db_t db = glb_conf::pInstance()->db();
	std::filesystem::path db_root_path = db.root;
	std::filesystem::path db_history_path = db_root_path / db.history_trade;
	sqlite::DB_t history_db(db_history_path.string());
	
	S4::sqlite::s4_history_trade_t_dbTbl history_tbl;
	std::string condition = " WHERE mktCodeStr = '" + stkName + "'";
	if (stgName.size()) {
		condition += " AND stgName = '" + stgName + "'";
	}
	history_db.read_table< S4::sqlite::s4_history_trade_t_dbTbl::data_t>(&history_tbl, table_name, history_trade_data, condition);

	LCL_INFO("loadOrdres stk={:} stg={:} tbl={:} done, size={:}", stkName, stgName, table_name, history_trade_data.size());

}

}//namespace QT
}//namespace S4