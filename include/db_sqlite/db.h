#pragma once

#include <iostream>
#include <set>
#include "common/s4logger.h"
#include "tableIO.h"
#include "common/s4exceptions.h"

namespace S4{
namespace sqlite{

class DB_t{
public:
    DB_t(const std::string & name, const int Mode= SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    //~DB_t();

    // void setConnection(const std::string & cnt);
	std::set<std::string> get_table_list(void);

	// col_name : col_type
	std::vector<std::string> get_colum_list(const std::string& table_name);

	void drop_table(const std::string& tbl_name);
	void drop_tables(const std::vector<std::string>& tbl_name);

    //drop_if_exist: drop table if exist
	template <typename T>
	void to_table(tableIO_t<T> * tableIO, const std::string& tbl_name, const std::vector<T>& data, bool drop_if_exist = true)
	{
		const size_t nb_insert = data.size();
		if (0 == nb_insert)
			return;
		
		tableIO->set_name(tbl_name);

		try
		{
			const bool bExists = mDb.tableExists(tbl_name);

			if (bExists) {
				if (drop_if_exist)
					mDb.exec("DROP TABLE IF EXISTS " + tbl_name);
				else {
					//mDb.exec("ALTER TABLE " + data->get_name() + " ADD COLUMN lcl_time INTEGER");
					//mDb.exec("ALTER TABLE " + data->get_name() + " ADD COLUMN lcl_time_str TEXT");
				}
			}

			if (!bExists || drop_if_exist)
				mDb.exec(tableIO->get_query_build());

			const std::string query_str = tableIO->get_query_insert();

			//TODO: rollback
			//int i = 0;
			//while (i < nb_insert) {
			//	{
			//		SQLite::Transaction transaction(mDb);
			//		for (int j = 0; j < 32768 && i < nb_insert; ++i, ++j) {
			//			SQLite::Statement   query(mDb, query_str);
			//			data->bind_query(query, i);
			//			query.exec();
			//		}
			//		transaction.commit();
			//	}
			//}
			SQLite::Transaction transaction(mDb);
			for (size_t i = 0; i < data.size(); ++i) {
				SQLite::Statement   query(mDb, query_str);
				tableIO->bind_query(query, data, i);
				query.exec();
			}
			transaction.commit();

			return;

		}
		catch (std::exception& e)
		{
			ERR("SQLite {} to_table {} failed: exception: {}", m_name, tbl_name, e.what());
			throw e;
			// exit(EXIT_FAILURE);//return; // unexpected error : exit the example program
		}
	}

	template <typename T>
    void read_table(tableIO_t<T> * tableIO, const std::string& tbl_name, std::vector<T>& data,
			 const std::string & condition="")
	{
		if (!mDb.tableExists(tbl_name))
			return;

		data.clear();

		tableIO->set_name(tbl_name);

		std::string queryStr;
		if (condition.find("select") > condition.size())
			queryStr = "select * from " + tbl_name + " " + condition;
		else
			queryStr = condition;
		try {
			SQLite::Statement   query(mDb, queryStr);

			while (query.executeStep()) {
				tableIO->load_query(query, data);
			}

			return;
		}
		catch (std::exception& e)
		{
			ERR("SQLite {} read_table {} failed: exception: {}", m_name, tbl_name, e.what());
			throw e;
			// exit(EXIT_FAILURE);//return; // unexpected error : exit the example program
		}
	}


	template <typename T>
	void to_table_v2(T * tableIO, const std::string& tbl_name, const std::vector<typename T::data_t>& data, bool drop_if_exist = true)
	{
		const size_t nb_insert = data.size();
		if (0 == nb_insert)
			return;
		
		tableIO->set_name(tbl_name);

		try
		{
			const bool bExists = mDb.tableExists(tbl_name);

			if (bExists) {
				if (drop_if_exist)
					mDb.exec("DROP TABLE IF EXISTS " + tbl_name);
				else {
					//mDb.exec("ALTER TABLE " + data->get_name() + " ADD COLUMN lcl_time INTEGER");
					//mDb.exec("ALTER TABLE " + data->get_name() + " ADD COLUMN lcl_time_str TEXT");
				}
			}

			if (!bExists || drop_if_exist)
				mDb.exec(tableIO->get_query_build());

			const std::string query_str = tableIO->get_query_insert();

			//TODO: rollback
			//int i = 0;
			//while (i < nb_insert) {
			//	{
			//		SQLite::Transaction transaction(mDb);
			//		for (int j = 0; j < 32768 && i < nb_insert; ++i, ++j) {
			//			SQLite::Statement   query(mDb, query_str);
			//			data->bind_query(query, i);
			//			query.exec();
			//		}
			//		transaction.commit();
			//	}
			//}
			SQLite::Transaction transaction(mDb);
			for (size_t i = 0; i < data.size(); ++i) {
				SQLite::Statement   query(mDb, query_str);
				tableIO->bind_query(query, data, i);
				query.exec();
			}
			transaction.commit();

			return;

		}
		catch (std::exception& e)
		{
			ERR("SQLite {} to_table {} failed: exception: {}", m_name, tbl_name, e.what());
			throw e;
			// exit(EXIT_FAILURE);//return; // unexpected error : exit the example program
		}
	}

	template <typename T>
    void read_table_v2(T * tableIO, const std::string& tbl_name, std::vector<typename T::data_t>& data,
			 const std::string & condition="")
	{
		if (!mDb.tableExists(tbl_name))
			return;

		data.clear();

		tableIO->set_name(tbl_name);

		std::string queryStr;
		if (condition.find("select") > condition.size())
			queryStr = "select * from " + tbl_name + " " + condition;
		else
			queryStr = condition;
		try {
			SQLite::Statement   query(mDb, queryStr);

			while (query.executeStep()) {
				tableIO->load_query(query, data);
			}

			return;
		}
		catch (std::exception& e)
		{
			ERR("SQLite {} read_table {} failed: exception: {}", m_name, tbl_name, e.what());
			throw e;
			// exit(EXIT_FAILURE);//return; // unexpected error : exit the example program
		}
	}
private:
    SQLite::Database  mDb;
    const std::string m_name;
};



class DB_group_t {
public:
	//DB_group_t(const std::string & path, const int Mode = SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) ;

	virtual ~DB_group_t()
	{}

	template <typename T>
	void to_table(tableIO_t<T> * tableIO, const std::string& tbl_name, std::vector<T>& data, bool drop_if_exist)
	{
		const std::string dbName = mapTable_to_db(tbl_name);
		if (_dbLib.count(dbName) == 0) {
			throw SqliteDBError("no such db in lib:" + dbName);
		}
		_dbLib.at(dbName)->to_table<T>(tableIO, tbl_name, data, drop_if_exist);
	}

	template <typename T>
    void read_table(tableIO_t<T> * tableIO, const std::string& tbl_name, std::vector<T>& data,
			 const std::string & condition="")
	{
		const std::string dbName = mapTable_to_db(tbl_name);
		if (_dbLib.count(dbName) == 0) {
			throw SqliteDBError("no such db in lib:" + dbName);
		}
		_dbLib.at(dbName)->read_table<T>(tableIO, tbl_name, data, condition);
	}

	virtual const std::string mapTable_to_db(const std::string&) const = 0;

protected:
	typedef std::map<const std::string, std::shared_ptr<DB_t>> dbLib_t;
	dbLib_t _dbLib;
};


class history_data_DB_group_t : public DB_group_t
{
public:
	history_data_DB_group_t(const std::string & path, const int Mode = SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE) ;

	virtual ~history_data_DB_group_t() {};

	virtual const std::string mapTable_to_db(const std::string&) const override;

};


}
}
