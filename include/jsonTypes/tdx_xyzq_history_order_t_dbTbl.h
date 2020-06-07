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
    "__assign_type_fields__": {{"field":"cpp-type"}}, # Assign specal cpp-type of field, but not infer automatically as default
    "__assign_set_lists__": [], # Take list in .json file as std::set<>, but not std::vector<> as default
    "__comment__xxx":"", # Add comment line
    "__sqlite_capable__":"", # enable sqlite tableIO autogen
* Script author: ChenZaihui<chinsaiki@outlook.com>
*/
#pragma once


#include "types/s4type.h"
#include "db_sqlite/tableIO.h"
#include <SQLiteCpp/ExecuteMany.h>

#include "jsonTypes/tdx_xyzq_history_order_t.h"

namespace S4 {
namespace sqlite {

class tdx_xyzq_history_order_t_dbTbl : public tableIO_t<struct tdx_xyzq_history_order_t>{
public:
    typedef struct tdx_xyzq_history_order_t data_t;
	//tdx_xyzq_history_order_t_dbTbl(const std::string name)
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
    
    virtual void bind_query(SQLite::Statement& query, const std::vector<struct tdx_xyzq_history_order_t>& data, size_t nb) override
    {
        const struct tdx_xyzq_history_order_t & K_data = data[nb];
        SQLite::bind(query,
			K_data.date,
			K_data.time_format,
			K_data.time_utcSec,
			K_data.stock_code,
			K_data.stock_name,
			K_data.id,
			K_data.opt_type,
			K_data.delegate_type,
			K_data.status,
			K_data.order_price,
			K_data.order_vol,
			K_data.deal_price,
			K_data.deal_vol,
			K_data.quote_mode);
    }

    //warning: not clear data inside, but append DB.data to it
    virtual void load_query(SQLite::Statement& query, std::vector<tdx_xyzq_history_order_t>& data) override
    {
        struct tdx_xyzq_history_order_t K_data;
        K_data.date = (time_date_t)query.getColumn(0).getInt64();
		K_data.time_format = query.getColumn(1).getString();
		K_data.time_utcSec = (time_utcSec_t)query.getColumn(2).getInt64();
		K_data.stock_code = query.getColumn(3).getString();
		K_data.stock_name = query.getColumn(4).getString();
		K_data.id = (int64_t)query.getColumn(5).getInt64();
		K_data.opt_type = query.getColumn(6).getString();
		K_data.delegate_type = query.getColumn(7).getString();
		K_data.status = query.getColumn(8).getString();
		K_data.order_price = (fprice_t)query.getColumn(9).getDouble();
		K_data.order_vol = (vol_share_t)query.getColumn(10).getInt64();
		K_data.deal_price = (fprice_t)query.getColumn(11).getDouble();
		K_data.deal_vol = (vol_share_t)query.getColumn(12).getInt64();
		K_data.quote_mode = query.getColumn(13).getString();
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
        "time_format	TEXT, "
        "time_utcSec	INTEGER, "
        "stock_code	TEXT, "
        "stock_name	TEXT, "
        "id	INTEGER, "
        "opt_type	TEXT, "
        "delegate_type	TEXT, "
        "status	TEXT, "
        "order_price	DOUBLE, "
        "order_vol	INTEGER, "
        "deal_price	DOUBLE, "
        "deal_vol	INTEGER, "
        "quote_mode	TEXT, "

        "PRIMARY KEY(id)"
    ")";



const std::string K_IN =
    "("
    "date, time_format, time_utcSec, stock_code, stock_name, id, opt_type, delegate_type, status, order_price, order_vol, deal_price, deal_vol, quote_mode"
    ") VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)"
;


};

}//namespace sqlite
}//namespace S4
