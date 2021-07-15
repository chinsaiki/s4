#include "db_sqlite/db.h"
#include "types/s4convertors.h"
#include "common/s4logger.h"

CREATE_LOCAL_LOGGER(sqlite::db)

namespace S4 {
namespace sqlite {

DB_t::DB_t(const std::string & name, const int Mode):
	mDb(name, Mode),
	m_name(name)
{
	//INFO( "SQLite database file '{}' opened successfully", mDb.getFilename().c_str());

	try {
		//mDb.exec("PRAGMA auto_vacuum = 1");
		mDb.exec("PRAGMA temp_store = MEMORY");
		mDb.exec("PRAGMA page_size = 65536");
		mDb.exec("PRAGMA cache_size = 8000");
		//mDb.exec("PRAGMA synchronous = OFF");
		// Test if the 'test' table exists
		// const bool bExists = db.tableExists("test");
		// std::cout << "SQLite table 'test' exists=" << bExists << "\n";
	}
	catch (std::exception & e) {
		LCL_ERR("sqlite::DB_t({:}) init fail: {:}", name, e.what());
		throw e;
	}
}


void DB_t::drop_table(const std::string& tbl_name)
{
	try {
		std::string queryStr("DROP TABLE ");

		SQLite::Statement   query(mDb, queryStr + tbl_name);

		while (query.executeStep());
	}
	catch (std::exception& e) {
		LCL_FATAL("sqlite::DB_t({:}) drop_table({:}) fail: {:}", m_name, tbl_name, e.what());
	}
	return;
}

void DB_t::drop_tables(const std::vector<std::string>& tbl_name)
{
	std::string queryStr("DROP TABLE ");
	for (const auto& t : tbl_name)
	{
		try {
			SQLite::Statement   query(mDb, queryStr + t);

			while (query.executeStep());
		}
		catch (std::exception& e) {
			LCL_FATAL("sqlite::DB_t({:}) drop_tables({:}) fail: {:}", m_name, t, e.what());
		}
	}
	return;
}

std::set<std::string> DB_t::get_table_list(void)
{
	std::set<std::string> ret;
	try {
		std::string queryStr("SELECT name FROM sqlite_master WHERE type ='table'");
		
		SQLite::Statement   query(mDb, queryStr);

		while (query.executeStep()) {
			ret.insert(query.getColumn(0).getString());
		}
	}
	catch (std::exception & e) {
		LCL_FATAL("sqlite::DB_t({:}) get_table_list fail: {:}", m_name, e.what());
	}
	return std::move(ret);
}

std::vector<std::string> DB_t::get_colum_list(const std::string& table_name)
{
	std::vector<std::string> ret;
	try {
		std::string queryStr("PRAGMA  table_info(" + table_name + ")");

		SQLite::Statement   query(mDb, queryStr);

		while (query.executeStep()) {
			std::string col_name = query.getColumn(1).getString();
			//std::string type = query.getColumn(2).getString();
			//ret[col_name] = type;
			ret.emplace_back(col_name);
		}
	}
	catch (std::exception & e) {
		LCL_FATAL("sqlite::DB_t({:}) get_colum_list fail: {:}", m_name, e.what());
	}
	return std::move(ret);
}


//s3stk_shIDX0.db3   : sh000000 ~ sh500000     //index, used=0~993
//s3stk_shETF0.db3   : sh500000 ~ sh600000     //etf, used=510010~sh519997; sh588000~sh588405
//s3stk_shSTK0.db3   : sh600000 ~ sh600999     //stk
//s3stk_shSTK1.db3   : sh601000 ~ sh603999     //stk
//s3stk_shSTK2.db3   : sh604000 ~ sh679999     //stk final ~ sh605577
//s3stk_shKCB0.db3   : sh680000 ~ sh689999     //kc 688000 ~ sh689009
//s3stk_shIDX1.db3   : sh690000 ~              //index, sh880001 ~ sh880993

//s3stk_szSTK0.db3 : sz000000 ~ sz002399       //stk
//s3stk_szSTK1.db3 : sz002400 ~ sz099999       //stk final ~ sz003816
//s3stk_szETF0.db3 : sz100000 ~ sz299999       //etf used=sz159001~sz169201
//s3stk_szCYB0.db3 : sz300000 ~ sz389999       //stk final ~ sz301040
//s3stk_szIDX0.db3 : sz390000 ~                //index, sz395001 ~ sz399998
history_data_DB_group_t::history_data_DB_group_t(const std::string & path, const int Mode)
{
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("shIDX0"), std::make_shared<DB_t>(path + "s4stk_shIDX0.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("shETF0"), std::make_shared<DB_t>(path + "s4stk_shETF0.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("shSTK0"), std::make_shared<DB_t>(path + "s4stk_shSTK0.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("shSTK1"), std::make_shared<DB_t>(path + "s4stk_shSTK1.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("shSTK2"), std::make_shared<DB_t>(path + "s4stk_shSTK2.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("shKCB0"), std::make_shared<DB_t>(path + "s4stk_shKCB0.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("shIDX1"), std::make_shared<DB_t>(path + "s4stk_shIDX1.db", Mode)));

	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("szSTK0"), std::make_shared<DB_t>(path + "s4stk_szSTK0.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("szSTK1"), std::make_shared<DB_t>(path + "s4stk_szSTK1.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("szETF0"), std::make_shared<DB_t>(path + "s4stk_szETF0.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("szCYB0"), std::make_shared<DB_t>(path + "s4stk_szCYB0.db", Mode)));
	_dbLib.insert(std::pair<const std::string, std::shared_ptr<DB_t>>(std::string("szIDX0"), std::make_shared<DB_t>(path + "s4stk_szIDX0.db", Mode)));
}

const std::string history_data_DB_group_t::mapTable_to_db(const std::string& tblName) const
{
	std::string mk = tblName.substr(0, 2);
	if (mk != "sz" && mk != "sh") {
		throw SqliteDBError("Unknow market name:" + mk + " in tableName:" + tblName);
	}
	int ins = static_cast<int>(IntConvertor::convert(tblName.substr(2, 8)));

	if (mk == "sh") {
		if (ins < 500000) {
			return "shIDX0";
		}
		else if (ins < 600000) {
			return "shETF0";
		}
		else if (ins < 600999) {
			return "shSTK0";
		}
		else if (ins < 603999) {
			return "shSTK1";
		}
		else if (ins < 679999) {
			return "shSTK2";
		}
		else if (ins < 689999) {
			return "shKCB0";
		}
        else {
            return "shIDX1";
        }
	}
	else if (mk == "sz") {
		if (ins < 2400) {
			return "szSTK0";
		}
		else if (ins < 99999) {
			return "szSTK1";
		}
		else if (ins < 299999) {
			return "szETF0";
		}
		else if (ins < 389999) {
			return "szCYB0";
		}
		else {
			return "szIDX0";
		}
	}

	throw SqliteDBError("Unknow tableName:" + tblName);
}

}
}