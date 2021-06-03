#include "trade/s3_history.h"
#include "common/s4time.h"
#include "common/s4logger.h"

CREATE_LOCAL_LOGGER(s3)

namespace S4{
namespace S3c{

bool read_s3history(sqlite::DB_t& history_db, const std::string& table_name, const std::string& mktCodeStr, std::vector<s3_order_t>& s3data)
{
	S4::sqlite::s3_order_t_dbTbl history_tbl;
	std::string condition;
	if (mktCodeStr.size()) {
		condition = " WHERE InsName = '" + mktCodeStr + "'";
	}

	history_db.read_table<S4::sqlite::s3_order_t_dbTbl::data_t>(&history_tbl, table_name, s3data, condition);

	return true;
}

bool trans_s3history(const std::vector<s3_order_t>& s3data, std::vector<s4_history_trade_t>& s4data)
{
	for (const auto& s3order : s3data) {
		s4_history_trade_t s4order;
		s4order.id = s3order.ID;
		s4order.date = utc_to_date(s3order.time);
		s4order.stgName = s3order.StgName;
		s4order.mktCodeStr = s3order.InsName;
		s4order.time_utcSec = s3order.time;
		s4order.datetime = s3order.datetime;
		if (s3order.optType == "Send") {
			s4order.optType = trade_opt_t::oSEND_OPEN;
		}
		else if (s3order.optType == "Open") {
			s4order.optType = trade_opt_t::oOPEN;
		}
		else if (s3order.optType == "Take") {
			s4order.optType = trade_opt_t::oTAKE;
		}
		else if (s3order.optType == "Stop") {
			s4order.optType = trade_opt_t::oSTOP;
		}
		else if (s3order.optType == "Close") {
			s4order.optType = trade_opt_t::oCLOSE;
		}
		else if (s3order.optType == "Abort") {
			s4order.optType = trade_opt_t::oABORT_OPEN;
		}
		else if (s3order.optType == "ChangeOpenTS") {
			s4order.optType = trade_opt_t::oCHANGE_OTS;
		}
		else if (s3order.optType == "ChangeTakeStop") {
			s4order.optType = trade_opt_t::oCHANGE_CTS;
		}
		else if (s3order.optType == "ChangeClose") {
			s4order.optType = trade_opt_t::oCHANGE_CTS;
		}
		else if (s3order.optType == "Unknow") {
			s4order.optType = trade_opt_t::oUNKNOWN_OPT;
		}
		else {
			LCL_ERR("unknown s3 opt={:}", s3order.optType);
		}
		s4order.position = "long";

		if (s3order.status == "NEW") {
			s4order.status = "new";
		}
		else if (s3order.status == "OPENED") {
			s4order.status = "opened";
		}
		else if (s3order.status == "CLOSED") {
			s4order.status = "closed";
		}
		else if (s3order.status == "TAKED") {
			s4order.status = "taked";
		}
		else if (s3order.status == "STOPPED") {
			s4order.status = "stopped";
		}
		else if (s3order.status == "ABORTED") {
			s4order.status = "aborted";
		}
		else if (s3order.status == "Unknow") {
			s4order.status = "Unknow";
		}
		else {
			LCL_ERR("unknown s3 status={:}", s3order.status);
		}

		s4order.order_open = s3order.open;
		s4order.order_take = s3order.take;
		s4order.order_stop = s3order.stop;
		s4order.order_close = s3order.close;
		s4order.deal_open = s3order.openA;
		s4order.deal_close = s3order.closeA;

		s4order.order_vol = 0;
		s4order.deal_vol = 0;
		s4order.deal_amt = 0;
		s4order.commission = 0;
		s4order.stamp_duty = 0;
		s4order.transfer_fee = 0;
		s4order.other_fees = 0;
		s4order.remarks = "s3";

		s4data.emplace_back(s4order);
	}
	return true;
}

} // namespace S3c
} // namespace S4