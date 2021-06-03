#include "common/s4logger.h"
#include "db_sqlite/db.h"
CREATE_LOCAL_LOGGER("drop_db_tables")

using namespace S4;
using namespace S4::sqlite;
using namespace std;


int main(int argc, char** argv)
{
	std::filesystem::path db_root_path = "G:\\E\\work\\999_s\\s4\\db";
	std::filesystem::path db_path = db_root_path / "s4_history_trade.db";
	sqlite::DB_t db(db_path.string());
	std::set<std::string>db_tables = db.get_table_list();

	std::string to_drop = "s3_REG_2020_07_05__11_49_27_cyc200";
	db.drop_table(to_drop);

	//std::vector<std::string> to_drop;

	//for (auto& s : db_tables) {
	//	if (s.find("_day") == s.size() - 4) {
	//		to_drop.push_back(s);
	//		LCL_WARN("will drop {:}.{:}", db_path.string(), s);
	//	}
	//}
	//cout << "enter Y to cofirm" << endl;
	//std::string arr;
	//cin >> arr;
	//cout << arr << endl;
	//if (arr != "Y") {
	//	LCL_INFO("stopped.");
	//	return 0;
	//}

	//int n = 10;
	//while (n>0)
	//{
	//	cout << "Ctrl + c to stop ... " << n << endl;
	//	n--;
	//	process_sleep(1);
	//}

	//db.drop_tables(to_drop);

	LCL_INFO("Drop done!");

	return 0;
}