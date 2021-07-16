/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: {0}
* Type name: {1}
* Tester:    int {1}_tester();
*
* Json keep-word: 
    "__default_value_fields__": [], # Take value in .json file as the default value of cpp variable
    "__optional_fields__": [], # Not require to present to .json file, but always in cpp struct
    "__assign_type_fields__": {{"field":"cpp-type"}}, # Assign specal cpp-type of field, but not infer automatically as default.
    "__assign_enum_fields__": {{"field":"enum-type"}}, # Assign specal enum-type of field, but not infer automatically as default.
                              enum-type need have implemented <enum-type>_toSting() & <enum-type>_fromString() functions.
    "__assign_set_lists__": [], # Take list in .json file as std::set<>, but not std::vector<> as default
    "__cpp_eq_eclude__":[],     # List of variables not involved in equal compare.
    "__comment__xxx":"", # Add comment line
    "__sqlite_capable__":"", # enable sqlite tableIO autogen
    "__sqlite_primary__":"", # assign the primary key of sqlite, if not assigned, first existing col of [ 'id', 'date', 'mktCode', 'datetime', 'code'] will be assigned automatically.
* Script author: ChinSaiki<chinsaiki@outlook.com>
*/
#pragma once


#include "types/s4type.h"
#include "db_sqlite/tableIO.h"
#include <SQLiteCpp/ExecuteMany.h>

#include "jsonTypes/sina_day_t.h"

namespace S4 {
namespace sqlite {

class sina_day_t_dbTbl : public tableIO_t<struct sina_day_t>{
public:
    typedef struct sina_day_t data_t;
	//sina_day_t_dbTbl(const std::string name)
    //{
    //    set_name(name);
    //};
    
	virtual void set_name(const std::string& name) override {
        m_name = name;
        m_qurey_build = "CREATE TABLE if not exists " + m_name + K_COL;
        m_qurey_insert = "INSERT OR IGNORE INTO " + m_name + K_IN;
    }

    virtual const std::string & get_query_build(void) const override { return m_qurey_build;};

    virtual const std::string & get_query_insert(void) const override { return m_qurey_insert;};
    
    virtual void bind_query(SQLite::Statement& query, const std::vector<struct sina_day_t>& data, size_t nb) override
    {
        const struct sina_day_t & K_data = data[nb];
        query.bind(1, K_data.date);
		query.bind(2, K_data.name);
		query.bind(3, K_data.trade);
		query.bind(4, K_data.pricechange);
		query.bind(5, K_data.changepercent);
		query.bind(6, K_data.buy);
		query.bind(7, K_data.sell);
		query.bind(8, K_data.settlement);
		query.bind(9, K_data.open);
		query.bind(10, K_data.high);
		query.bind(11, K_data.low);
		query.bind(12, K_data.volume);
		query.bind(13, K_data.amount);
		query.bind(14, K_data.ticktime);
		query.bind(15, K_data.per);
		query.bind(16, K_data.pb);
		query.bind(17, K_data.mktcap);
		query.bind(18, K_data.nmc);
		query.bind(19, K_data.turnoverratio);
    }

    //warning: not clear data inside, but append DB.data to it
    virtual void load_query(SQLite::Statement& query, std::vector<sina_day_t>& data) override
    {
        struct sina_day_t K_data;
        K_data.date = (time_date_t)query.getColumn(0).getInt64();
		K_data.name = (std::string)query.getColumn(1).getString();
		K_data.trade = (fprice_t)query.getColumn(2).getDouble();
		K_data.pricechange = (fprice_t)query.getColumn(3).getDouble();
		K_data.changepercent = (double)query.getColumn(4).getDouble();
		K_data.buy = (fprice_t)query.getColumn(5).getDouble();
		K_data.sell = (fprice_t)query.getColumn(6).getDouble();
		K_data.settlement = (fprice_t)query.getColumn(7).getDouble();
		K_data.open = (fprice_t)query.getColumn(8).getDouble();
		K_data.high = (fprice_t)query.getColumn(9).getDouble();
		K_data.low = (fprice_t)query.getColumn(10).getDouble();
		K_data.volume = (vol_share_t)query.getColumn(11).getInt64();
		K_data.amount = (amount_t)query.getColumn(12).getInt64();
		K_data.ticktime = (std::string)query.getColumn(13).getString();
		K_data.per = (double)query.getColumn(14).getDouble();
		K_data.pb = (double)query.getColumn(15).getDouble();
		K_data.mktcap = (double)query.getColumn(16).getDouble();
		K_data.nmc = (double)query.getColumn(17).getDouble();
		K_data.turnoverratio = (double)query.getColumn(18).getDouble();
        data.push_back(std::move(K_data));
    }

private:
	//std::string m_name;
    std::string m_qurey_build;
    std::string m_qurey_insert;
private:

const std::string K_COL =
    "( "
        "date	INTEGER, "
        "name	TEXT, "
        "trade	DOUBLE, "
        "pricechange	DOUBLE, "
        "changepercent	DOUBLE, "
        "buy	DOUBLE, "
        "sell	DOUBLE, "
        "settlement	DOUBLE, "
        "open	DOUBLE, "
        "high	DOUBLE, "
        "low	DOUBLE, "
        "volume	INTEGER, "
        "amount	INTEGER, "
        "ticktime	TEXT, "
        "per	DOUBLE, "
        "pb	DOUBLE, "
        "mktcap	DOUBLE, "
        "nmc	DOUBLE, "
        "turnoverratio	DOUBLE, "

        "PRIMARY KEY(date)"
    ")";



const std::string K_IN =
    "("
    "date, name, trade, pricechange, changepercent, buy, sell, settlement, open, high, low, volume, amount, ticktime, per, pb, mktcap, nmc, turnoverratio"
    ") VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"
;


};

}//namespace sqlite
}//namespace S4
