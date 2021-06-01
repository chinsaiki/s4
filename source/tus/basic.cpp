
#include "tus/basic.h"
#include "tus/cons.h"
#include "common/s4logger.h"
#include "common/s4string.h"
#include "types/s4convertors.h"
#include "common/s4conf.h"

#ifdef max
#undef max
#endif
#include "tus/httplib.h"
using namespace std;

CREATE_LOCAL_LOGGER("tus_basic")

namespace S4{
namespace TUS{

static
bool convCSVtoBasic(const std::string& csvStr, std::vector<tushare_basic_t>& basicOfDay);

bool downloadBasic(const std::string& date, std::vector<tushare_basic_t>& basicOfDay)
{
	basicOfDay.clear();

	char url[128];
	if(date.size()!=8){
		sprintf(url, URL_BASIC_URL_TODAY);
		LCL_INFO("downloading basic.csv of TODAY from {:}", date, url);
	}else{
		sprintf(url, URL_BASIC_URL_SOMEDAY, date.substr(0,6).c_str(), date.c_str());
		LCL_INFO("downloading basic.csv of {} from {:}", date, url);
	}

	httplib::Client m_tushare("file.tushare.org", 80);
	m_tushare.set_timeout_sec(20);

	std::shared_ptr<httplib::Response> res =
		m_tushare.Get(url, [](uint64_t len, uint64_t total) {
		printf("\r%d / %d bytes => %d%% complete",
			(int)len, (int)total,
			(int)((len * 100 / total)));
		return true; // return 'false' if you want to cancel the request.
	}
	);
	printf("\n");

	if (res==NULL || res->status != 200) {
		LCL_ERR("downloaded {} failed!", url);
		if (res) {
			LCL_ERR(" ret={} ", res->status);
		}
		else {
			LCL_ERR(" res = NULL! ");
		}
		return false;
	}
	//std::cout << res->body << std::endl;
	write_string_to_file(".\\basic_" + date + ".csv", res->body, true);
	LCL_INFO("basic saved.");

	return convCSVtoBasic(res->body, basicOfDay);

}

bool basic_to_db(sqlite::DB_t& basic_db, const std::string& date, const std::vector<tushare_basic_t>& basicOfDay, bool check)
{
	std::string table_name = "tus_" + date;

	S4::sqlite::tushare_basic_t_dbTbl basic_tbl;
	basic_db.to_table<S4::sqlite::tushare_basic_t_dbTbl::data_t>(&basic_tbl, table_name, basicOfDay);

	if (check){
		std::vector<S4::tushare_basic_t> basic_rd;
		basic_db.read_table< S4::sqlite::tushare_basic_t_dbTbl::data_t>(&basic_tbl, table_name, basic_rd);

		if (basicOfDay.size() != basic_rd.size()) {
			LCL_ERR("deal DB write - read unequal! w.size={:}, r.size={:}", basicOfDay.size(), basic_rd.size());
		}
		for (size_t i = 0; i < basic_rd.size(); ++i) {
			bool match = false;
			for (size_t j = 0; j < basicOfDay.size(); ++j) {
				if (basicOfDay[j] == basic_rd[i]) {
					//LCL_ERR("deal DB write - read unequal! w[{:}] != r[{:}]", i, i);		//as example, 8 and 9 are unequal for reordered by DB
					match = true;
					break;
				}
			}
			if (!match) {
				LCL_ERR("deal DB write - read NG! r[{:}] not found from w", i);
				return false;
			}
		}
	}
	return true;
}

bool basic_from_db(sqlite::DB_t& basic_db, const std::string& date, std::vector<tushare_basic_t>& basicOfDay)
{
	basicOfDay.clear();
	std::string table_name = "tus_" + date;

	S4::sqlite::tushare_basic_t_dbTbl basic_tbl;
	basic_db.read_table< S4::sqlite::tushare_basic_t_dbTbl::data_t>(&basic_tbl, table_name, basicOfDay);

	return true;
}

bool download_basic_to_db(const std::string& date, bool try_today, bool check)
{
	std::vector<tushare_basic_t> basicOfDay;
	if (!downloadBasic(date, basicOfDay)){
		if (!try_today || !!downloadBasic("", basicOfDay))
			return false;
	}

	glb_conf_ctx_t::db_t db = glb_conf::pInstance()->db();
	std::filesystem::path db_root_path = db.root;
	std::filesystem::path db_tus_basic_path = db_root_path / db.tus_basic;
	sqlite::DB_t basic_db(db_tus_basic_path.string());

	return basic_to_db(basic_db, date, basicOfDay, check);
}

static
bool convCSVtoBasic(const std::string& csvStr, std::vector<tushare_basic_t>& basicOfDay)
{
	int cnt(0);
	size_t bgn(0), end(0);
	string line;

	basicOfDay.clear();

	bgn = csvStr.find('\n', bgn) + 1;	//skip first line
	while (end < csvStr.size()) {
		end = csvStr.find('\n', bgn);
		line = csvStr.substr(bgn, end - bgn - 1);
		bgn = end + 1;

		//cout << line << endl;
		if (line.size() < 10)
			continue;

		int lcnt(0);
		size_t lbgn(0), lend(0);
		tushare_basic_t basic;
		try {

			do {
				lend = line.find(',', lbgn);
				switch (lcnt++) {
				case 0: basic.code = (pureCodeI_t)IntConvertor::convert(line.substr(lbgn, lend - lbgn)); break;		// 代码
				case 1: basic.name = line.substr(lbgn, lend - lbgn); break;		//名称
				case 2: basic.industry = line.substr(lbgn, lend - lbgn); break;	//细分行业
				case 3: basic.area = line.substr(lbgn, lend - lbgn); break;		//地区
				case 4: basic.pe = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;				//市盈率
				case 5: basic.outstanding = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;		//流通股本(亿)
				case 6: basic.totals = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;			//总股本(亿)
				case 7: basic.totalAssets = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;		//总资产(万)
				case 8: basic.liquidAssets = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;		//流动资产
				case 9: basic.fixedAssets = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;		//固定资产
				case 10: basic.reserved = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;			//公积金
				case 11: basic.reservedPerShare = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;	//每股公积金
				case 12: basic.esp = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;				//每股收益
				case 13: basic.bvps = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;				//每股净资
				case 14: basic.pb = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;				//市净率
				case 15: basic.timeToMarket = (time_date_t)std::atoi(line.substr(lbgn, lend - lbgn).c_str()); break;			//上市日期	//tushare某几日此字段为浮点
				case 16: basic.undp = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;				//未分利润
				case 17: basic.perundp = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;			//每股未分配
				case 18: basic.rev = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;				//收入同比(%)
				case 19: basic.profit = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;			//利润同比(%)
				case 20: basic.gpr = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;				//毛利率(%)
				case 21: basic.npr = DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;				//净利润率(%)
				case 22: basic.holders = (int)DoubleConvertor::convert(line.substr(lbgn, lend - lbgn)); break;			//股东人数
				default:break;
				}
				lbgn = lend + 1;
			} while (lend < line.size());

		}
		catch (exception& e) {
			LCL_ERR("field decode error:{:}", e.what());
			return false;
		}
		if (basic.timeToMarket!=0 && !chk_stk_date_legal(basic.timeToMarket)) {
			LCL_ERR("timeToMarket = {} illegal!", basic.timeToMarket);
			return false;
		}

		if (lcnt != 23) continue;

		basicOfDay.emplace_back(move(basic));
		cnt++;
	}
	LCL_INFO("found {:d} stock from basic CSV", cnt);
	return cnt > 0;
}


}//namespace tus
}//namespace S4
