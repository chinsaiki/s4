#include <fstream>
#include "common/s4string.h"
#include "tdx/read_history.h"
#include "common/s4logger.h"
#include "jsonTypes/tdx_xyzq_history_deal_t.h"
#include "jsonTypes/tdx_xyzq_history_order_t.h"
#include "types/s4convertors.h"
#include "common/s4mktCode.h"
#include "common/s4time.h"

CREATE_LOCAL_LOGGER(tdx_read_history)

using namespace std;

#define ORDER_INDEX_PLEACEHOLDER (10000ll)

namespace S4{
namespace TDX{


//可能跳过的成交记录：打新配号、股息
bool read_history_deal(const std::string& file_name, std::vector<tdx_xyzq_history_deal_t>& deals)
{
	deals.clear();

    std::ifstream	reader(file_name);

    if(!reader.is_open()){
        LCL_ERR("can't open deal file = {:}", file_name);
        return false;
    }

    string str;
    getline(reader,str); // title
    vector<string> title = string_strip(str, "\t");
    while( getline(reader,str))
    {
        vector<string> lineV = string_strip(str, "\t");
		if (lineV.size() < title.size())
			continue;
		struct tdx_xyzq_history_deal_t deal;

		try {
			int deal_HMS = 0;
			for (size_t i = 0; i < lineV.size(); ++i) {
				const string v = lineV[i];
				const string t = title[i];
				if (t == "成交日期") {       deal.date = (time_date_t)IntConvertor::convert(v);
				}
				else if (t == "成交时间") {
					vector<string> time = string_strip(v, ":");
					if (time.size() != 3) {
						LCL_ERR("成交时间 decode size={:} != 3", time.size());
					}
					int hour = (int)IntConvertor::convert(time[0]);
					int minu = (int)IntConvertor::convert(time[1]);
					int sec = (int)IntConvertor::convert(time[2]);
					deal_HMS = hour * 10000 + minu * 100 + sec;
				}else if (t == "证券代码") { deal.stock_code = pureCodeInt_to_pureCodeStr((int)IntConvertor::convert(v));	//pureCodeStr
				}else if (t == "证券名称") { deal.stock_name = v;
				}else if (t == "买卖标志") { deal.opt_type = v;
				}else if (t == "成交价格") { deal.deal_price = (fprice_t)DoubleConvertor::convert(v);
				}else if (t == "成交数量") { deal.deal_vol = (vol_share_t)abs(IntConvertor::convert(v));
				}else if (t == "委托编号") { deal.id = (int)IntConvertor::convert(v);
				}else if (t == "成交金额") { deal.deal_amount = (amount_t)DoubleConvertor::convert(v);
				}else if (t == "佣金") {    deal.commission = (amount_t)DoubleConvertor::convert(v);
				}else if (t == "印花税") {	deal.stamp_duty = (amount_t)DoubleConvertor::convert(v);
				}else if (t == "过户费") {	deal.transfer_fee = (amount_t)DoubleConvertor::convert(v);
				}else if (t == "其他费") {	deal.other_fees = (amount_t)DoubleConvertor::convert(v);
				}else if (t == "备注") {    deal.remarks = v + "成交";
				}
			}
			mktCodeI_t mktCode = pureCodeStr_to_mktCode(deal.stock_code);
			if (deal_HMS == 0 || !isDealTime_stock(deal_HMS, mktCode)) {
				LCL_WARN("drop deal line for not-market-time:\n{:}", str);
				continue;
			}
			if (deal.deal_price == (fprice_t)0){
				LCL_WARN("drop deal line for price 0:\n{:}", str);
				continue;
			}
			deal.time_utcSec = date_to_utc(deal.date, deal_HMS);
			deal.time_format = utc_to_str(deal.time_utcSec);
			deal.id = deal.date * ORDER_INDEX_PLEACEHOLDER + deal.id;
			deals.emplace_back(move(deal));
		}
		catch (std::exception& e) {
			LCL_ERR("dec deal fail for {:}:\n{:}", e.what(), str);
			continue;
		}
    }

    return true;
}

bool read_history_order(const std::string& file_name, std::vector<tdx_xyzq_history_order_t>& orders)
{
	orders.clear();

    std::ifstream	reader(file_name);

    if(!reader.is_open()){
        LCL_ERR("can't open order file = {:}", file_name);
        return false;
    }

    string str;
    getline(reader,str); // title
    vector<string> title = string_strip(str, "\t");
    while( getline(reader,str))
    {
        vector<string> lineV = string_strip(str, "\t");
		if (lineV.size() < title.size())
			continue;
		struct tdx_xyzq_history_order_t order;

		try {
			int order_HMS = 0;
			for (size_t i = 0; i < lineV.size(); ++i) {
				const string v = lineV[i];
				const string t = title[i];
				if (t == "委托日期") {       order.date = (time_date_t)IntConvertor::convert(v);
				}
				else if (t == "委托时间") {
					vector<string> time = string_strip(v, ":");
					if (time.size() != 3) {
						LCL_ERR("委托时间 decode size={:} != 3", time.size());
					}
					int hour = (int)IntConvertor::convert(time[0]);
					int minu = (int)IntConvertor::convert(time[1]);
					int sec = (int)IntConvertor::convert(time[2]);
					order_HMS = hour * 10000 + minu * 100 + sec;
				}else if (t == "证券代码") { order.stock_code = pureCodeInt_to_pureCodeStr((int)IntConvertor::convert(v));	//pureCodeStr
				}else if (t == "证券名称") { order.stock_name = v;
				}else if (t == "买卖标志") { order.opt_type = v;
				}else if (t == "委托类别") { order.delegate_type = v;
				}else if (t == "状态说明") { order.status = v;
				}else if (t == "委托价格") { order.order_price = (fprice_t)DoubleConvertor::convert(v);
				}else if (t == "委托数量") { order.order_vol = (vol_share_t)abs(IntConvertor::convert(v));
				}else if (t == "成交价格") { order.deal_price = (fprice_t)DoubleConvertor::convert(v);
				}else if (t == "成交数量") { order.deal_vol = (vol_share_t)abs(IntConvertor::convert(v));
				}else if (t == "委托编号") { order.id = (int)IntConvertor::convert(v);
				}else if (t == "报价方式") { order.quote_mode = v + "下单";
				}
			}
			if (order.order_price == (fprice_t)0){
				LCL_WARN("drop order line for price 0:\n{:}", str);
				continue;
			}
			if (order.quote_mode != "买卖下单") {
				LCL_WARN("drop order line for quote_mode = {:}:\n{:}", order.quote_mode, str);
				continue;
			}
			order.time_utcSec = date_to_utc(order.date, order_HMS);
			order.time_format = utc_to_str(order.time_utcSec);
			order.id = order.date * ORDER_INDEX_PLEACEHOLDER + order.id;
			orders.emplace_back(move(order));
		}
		catch (std::exception& e) {
			LCL_ERR("dec deal fail for {:}:\n{:}", e.what(), str);
			continue;
		}
    }

    return true;
}

bool history_deal_to_DB(S4::sqlite::DB_t& history_db, const std::string& file_name, const std::string& table_name)
{
	std::vector<S4::tdx_xyzq_history_deal_t> deals;
	bool ret;
	ret = S4::TDX::read_history_deal(file_name, deals);
	if (!ret) {
		return false;
	}

	S4::sqlite::tdx_xyzq_history_deal_t_dbTbl deal_tbl;
	history_db.to_table<S4::sqlite::tdx_xyzq_history_deal_t_dbTbl::data_t>(&deal_tbl, table_name, deals);

	std::vector<S4::tdx_xyzq_history_deal_t> deals_rd;
	history_db.read_table< S4::sqlite::tdx_xyzq_history_deal_t_dbTbl::data_t>(&deal_tbl, table_name, deals_rd);

	if (deals.size() != deals_rd.size()) {
		LCL_ERR("deal DB write - read unequal! w.size={:}, r.size={:}", deals.size(), deals_rd.size());
	}
	for (size_t i = 0; i < min(deals.size(), deals_rd.size()); ++i) {
		if (deals[i] != deals_rd[i]) {
			LCL_ERR("deal DB write - read unequal! w[{:}] != r[{:}]", i, i);		//as example, 8 and 9 are unequal for reordered by DB
		}
		else {
			LCL_INFO("deal DB write - read OK! w[{:}] == r[{:}]", i, i);
		}
	}
	return true;
}

bool history_order_to_DB(S4::sqlite::DB_t& history_db, const std::string& file_name, const std::string& table_name)
{
	std::vector<S4::tdx_xyzq_history_order_t> orders;
	bool ret;
	ret = S4::TDX::read_history_order(file_name, orders);
	if (!ret) {
		return false;
	}

	S4::sqlite::tdx_xyzq_history_order_t_dbTbl order_tbl;
	history_db.to_table<S4::sqlite::tdx_xyzq_history_order_t_dbTbl::data_t>(&order_tbl, table_name, orders);

	std::vector<S4::tdx_xyzq_history_order_t> orders_rd;
	history_db.read_table< S4::sqlite::tdx_xyzq_history_order_t_dbTbl::data_t>(&order_tbl, table_name, orders_rd);

	if (orders.size() != orders_rd.size()) {
		LCL_ERR("order DB write - read unequal! w.size={:}, r.size={:}", orders.size(), orders_rd.size());
	}
	for (size_t i = 0; i < min(orders.size(), orders_rd.size()); ++i) {
		if (orders[i] != orders_rd[i]) {
			LCL_ERR("order DB write - read unequal! w[{:}] != r[{:}]", i, i);
		}
		else {
			LCL_INFO("order DB write - read OK! w[{:}] == r[{:}]", i, i);
		}
	}

	return true;

}



static
void merge_history_order(const std::vector<S4::tdx_xyzq_history_order_t>& orders_rd, std::vector<s4_history_trade_t>& history_trade_data)
{
	for(const auto& order : orders_rd){
		s4_history_trade_t data;
		//id through open-close
		data.id = order.id;
		data.date = order.date;
		//strategy name
		data.stgName = "tdx_xyzq_history";	//	tdx_xyzq
		data.mktCodeStr = pureCodeStr_to_mktCodeStr(order.stock_code);	//	sz000001
		data.time_utcSec = order.time_utcSec;	//	123
		data.datetime = utc_to_str(order.time_utcSec);	//	2018_04_26__00_00_00
		//current option of id: send_open / abort_open / send_close / abort_clsoe
		//current status of id: new / aborted / opened
		if (order.opt_type == "买入") {
			if (order.delegate_type == "撤单") {
				data.optType = trade_opt_t::oABORT_OPEN; //"abort_open";
				if (order.status == "已成") {
					data.status = "aborted";
				}
				else {
					data.status = "new";
				}
			}
			else {
				data.optType = trade_opt_t::oSEND_OPEN;//"send_open";
				data.status = "new";	//	new
			}
			data.order_open = fPrice_to_iPrice(order.order_price);	//	-1
			data.order_close = -1;	//	-1
			data.order_vol = order.order_vol;	//	-1
		}
		else if (order.opt_type == "卖出") {
			if (order.delegate_type == "撤单") {
				data.optType = trade_opt_t::oABORT_CLOSE; //"abort_close";
			}
			else {
				data.optType = trade_opt_t::oSEND_CLOSE;// "send_close";
			}
			data.status = "opened";
			data.order_open = -1;	//	-1
			data.order_close = fPrice_to_iPrice(order.order_price);
			data.order_vol = order.order_vol;	//	-1
		}
		else {
			continue;
		}
		//long as stock only for now
		data.position = "long";	//	long
		data.order_take = -1;	//	-1
		data.order_stop = -1;	//	-1

		data.deal_open = -1;
		data.deal_close = -1;	//	-1
		//not in use for now
		data.deal_vol = 0;	//	-1
		data.deal_amt = -1.0;	//	
		data.commission = 0.0;	//	0.0
		data.stamp_duty = 0.0;	//	0.0
		data.transfer_fee = 0.0;	//	0.0
		data.other_fees = 0.0;	//	0.0
		data.remarks = order.quote_mode;	//	起始配号:226168906

		history_trade_data.emplace_back(data);

	}
}


static
void merge_history_deal(const std::vector<S4::tdx_xyzq_history_deal_t>& deals_rd, std::vector<s4_history_trade_t>& history_trade_data)
{
	for (const auto& deal : deals_rd) {
		s4_history_trade_t data;
		//id through open-close
		data.id = deal.id;
		data.date = deal.date;
		//strategy name
		data.stgName = "tdx_xyzq_history";	//	tdx_xyzq
		data.mktCodeStr = pureCodeStr_to_mktCodeStr(deal.stock_code);	//	sz000001
		data.time_utcSec = deal.time_utcSec;	//	123
		data.datetime = utc_to_str(deal.time_utcSec);	//	2018_04_26__00_00_00
		//current option of id: open / close
		//current status of id: new->opened / opened->closed
		if (deal.opt_type == "买入") {
			data.optType = trade_opt_t::oOPEN;// "open";
			data.status = "opened";	//	new
			data.deal_open = fPrice_to_iPrice(deal.deal_price);
			data.deal_close = -1;	//	-1
		}
		else if (deal.opt_type == "卖出") {
			data.optType = trade_opt_t::oCLOSE; //"close";
			data.status = "closed";	//	new
			data.deal_open = -1;	//	-1
			data.deal_close = fPrice_to_iPrice(deal.deal_price);
		}
		else {
			continue;
		}
		//long as stock only for now
		data.position = "long";	//	long
		data.order_open = -1;	//	-1
		data.order_take = -1;	//	-1
		data.order_stop = -1;	//	-1
		data.order_close = -1;	//	-1
		data.order_vol = 0;	//	-1

		//not in use for now
		data.deal_vol = deal.deal_vol;	//	-1
		data.deal_amt = deal.deal_amount;	//	-1.0
		data.commission = deal.commission;	//	0.0
		data.stamp_duty = deal.stamp_duty;	//	0.0
		data.transfer_fee = deal.transfer_fee;	//	0.0
		data.other_fees = deal.other_fees;	//	0.0
		data.remarks = deal.remarks;	//	起始配号:226168906

		history_trade_data.emplace_back(data);

	}
}

bool read_history_DB(const std::filesystem::path& db_file_path, std::vector<s4_history_trade_t>& history_trade_data, 
	const std::set<std::string>& table_list, const std::vector<pureCodeI_t>& stk_list)
{
	history_trade_data.clear();
	sqlite::DB_t db(db_file_path.string(), SQLite::OPEN_READONLY);

	std::string condition = "";
	if (stk_list.size()) {
		condition = " WHERE stock_code = " + pureCodeInt_to_pureCodeStr(stk_list[0]);
		for (size_t i = 1; i < stk_list.size(); ++i) {
			condition += " OR stock_code = " + pureCodeInt_to_pureCodeStr(stk_list[i]);
		}
	}

	S4::sqlite::tdx_xyzq_history_deal_t_dbTbl deal_tbl;
	std::vector<S4::tdx_xyzq_history_deal_t> deals_rd;
	S4::sqlite::tdx_xyzq_history_order_t_dbTbl order_tbl;
	std::vector<S4::tdx_xyzq_history_order_t> orders_rd;

	std::vector<std::string> tables_in_db = db.get_table_list();

	for(auto& table : tables_in_db){
		if(table_list.size() && table_list.count(table) == 0){
			continue;
		}
		
		std::vector<std::string> colums = db.get_colum_list(table);
		bool has_deal_price = false;
		bool has_commission = false;
		bool has_stamp_duty = false;
		bool has_delegate_type = false;
		bool has_order_vol = false;
		for(auto& c : colums){
			if (c == "deal_price") has_deal_price = true;
			if (c == "commission") has_commission = true;
			if (c == "stamp_duty") has_stamp_duty = true;
			if (c == "delegate_type") has_delegate_type = true;
			if (c == "order_vol") has_order_vol = true;
		}
		//not deal nor order
		if (!has_deal_price){
			LCL_WARN("tdx history DB {:} contains non-history table: {:}", db_file_path.string(), table);
			continue;
		}
		//check is deal or order
		int table_type;
		if(has_commission && has_stamp_duty){
			table_type = 0;	//deal
		}else if(has_delegate_type && has_order_vol){
			table_type = 1;	//order
		}else{
			LCL_WARN("tdx history DB {:} contains non-history table: {:}", db_file_path.string(), table);
			continue;
		}

		if(table_type == 0){
			db.read_table<tdx_xyzq_history_deal_t>(&deal_tbl, table, deals_rd, condition);
			merge_history_deal(deals_rd, history_trade_data);
		}else{
			db.read_table<tdx_xyzq_history_order_t>(&order_tbl, table, orders_rd, condition);
			merge_history_order(orders_rd, history_trade_data);
		}

	}


	return true;
}

} // namespace TDX
} // namespace S4

