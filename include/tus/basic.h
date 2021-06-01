
#pragma once

#ifdef _WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

#include "jsonTypes/tushare_basic_t.h"
#include "jsonTypes/tushare_basic_t_dbTbl.h"

#include "db_sqlite/db.h"




namespace S4{
namespace TUS{

bool downloadBasic(const std::string& date, std::vector<tushare_basic_t>& basicOfDay);

bool basic_to_db(sqlite::DB_t& basic_db, const std::string& date, const std::vector<tushare_basic_t>& basicOfDay, bool check = false);

bool basic_from_db(sqlite::DB_t& basic_db, const std::string& date, std::vector<tushare_basic_t>& basicOfDay);

//if try_today, then will try to download today.csv when date.csv is not valid.
bool download_basic_to_db(const std::string& date, bool try_today = false, bool check = true);

}//namespace TUS
}//namespace S4
