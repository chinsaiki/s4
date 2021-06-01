#pragma once
#include "common/s4thread.h"
#include "common/s4exceptions.h"
#include "tdx/tdx_local.h"
#include "db_sqlite/db.h"
// #include "db_sqlite/tableIO_dayK.h"
// #include "db_sqlite/tableIO_minuK.h"
#include "data/s4info.h"
// #include "data/s4infCYC.h"
#include "data/s4infK.h"
#include "data/s4infMA.h"
// #include "data/s4infSnap.h"
#include "common/s4mktCode.h"

#include "common/s4conf.h"
// #include "tdx/tdx_ol.h"

#include <time.h>
#include <vector>
#include <string>
#include <memory>
#include<algorithm>

namespace S4 {

//struct OLreq_t
//{
//	bool day_need;
//	int day_bgnNum;
//	int day_endNum;
//
//	bool minu_need;
//	int minu_bgnNum;
//	int minu_endNum;
//
//	bool quote_need;
//};

struct stkInfoReq_t {
	// int bgnDate;
	int endDate;
	int nbDay_preEndDate;

	// int bgnMinuDate;	//read minu in unit of day
	int endMinuDate = 0;	//YYYYMMHH
	int nbMinu_preEndMinuDate = 0;	//count in minu
	bool needMinu = false;

	//std::vector<int> cyc_scope_list;
	std::vector<int> ma_scope_list;

	bool needSnap = false;
	//std::set<int>  snapDates;	//YYYYMMHH
	//int snap_lmt_Qsize = 0;

	//bool needTick;	//for future
	bool operator ==(const stkInfoReq_t& d) const
	{
		if (endDate == d.endDate &&
			nbDay_preEndDate == d.nbDay_preEndDate &&
			endMinuDate == d.endMinuDate &&
			nbMinu_preEndMinuDate == d.nbMinu_preEndMinuDate &&
			needMinu == d.needMinu &&
			needSnap == d.needSnap &&
			ma_scope_list.size() == d.ma_scope_list.size()
			)
		{
			for (size_t i = 0; i < ma_scope_list.size(); ++i) {
				bool match = false;
				for (size_t j = 0; j < ma_scope_list.size(); j++)
				{
					if (ma_scope_list[i] == d.ma_scope_list[j]) {
						match = true;
						break;
					}
				}
				if (!match) {
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool operator !=(const stkInfoReq_t& d) const
	{
		return !((*this) == d);
	}
};

class stkInfo_t {
public:
	stkInfo_t() {};

	explicit stkInfo_t(const std::string&);
	~stkInfo_t();

	std::shared_ptr<infKQ_t> pDayKQ;
	// std::shared_ptr<std::map<int, std::shared_ptr<CYCQ_t>>> pCycQlib;

	//MA
	std::shared_ptr<std::map<int, std::shared_ptr<maQ_t>>> pMAlib;	//MA

	std::shared_ptr<infKQ_t> pMinuKQ;
	std::string name() const {
		return _name;
	}
	mktCodeI_t mkCode() const {
		return _mkCodeInt;
	}

private:
	std::string _name;
	mktCodeI_t _mkCodeInt;

	//tick
	// std::shared_ptr<infSnapQ_t> pSnanpQ;

public:
	void doOTF(void);

	// void setBase(time_t now);
	// void nextBase(time_t clk);
	
	void setBase(time_t dayClk, time_t dayMinuClk);
	void nextBase(time_t dayClk, time_t dayMinuClk);

	void setBaseDay(time_t dayClk);
	void nextBaseDay(time_t dayClk);

	void setBaseInDay(time_t dayMinuClk);
	void nextBaseInDay(time_t dayMinuClk);

	//
	void addInfo(stkInfo_t& newInfo);

	/* DAY tool */
	const infK_t* getDay(int i) const;

	const infK_t* getDay_abs(time_t t) const{
		return pDayKQ->getInfo_abs(t);
	}

	int getDay_max_high(int bgn, int end);
	int getDay_min_low(int bgn, int end);
	int getDay_max_avg(int bgn, int end);
	int getDay_min_avg(int bgn, int end);

	int getDay_max_high_fq(int bgn, int end);
	int getDay_min_low_fq(int bgn, int end);
	int getDay_max_avg_fq(int bgn, int end);
	int getDay_min_avg_fq(int bgn, int end);

	int getDay_max_close(int bgn, int end);
	int getDay_max_close_fq(int bgn, int end);

	vol_share_t getDay_max_vol(int bgn, int end);
	vol_share_t getDay_min_vol(int bgn, int end);


	// cyc_t* getCyc(int scope, int i);
	// CYCQ_t* getCycQ(int scope);
	// int getCyc_max_avg(int scope, int bgn, int end);

	const ma_t* getMA(int scope, int i) const;
	int getMA_max(int scope, int bgn, int end);
	int getMA_min(int scope, int bgn, int end);

	//count if (day[i+iGap].close < price && day[i-iGap].close < price && day[i].high > price) where i in [bgn+1, bgn + len]
	int dayNb_Cross(int price, int bgn, int len, int iGap=1) const;

	/* Minu tool */
	const infK_t* getMinu(int i) const;

	/* SNAP tool */
	// infSnap_t* getSnap(int i);
	// infSnap_t* getSnap_before(time_t dlt, time_t error = 5);
	//time_t getSnap_OCHLAV(time_t dlt, int& O, int& C, int& H, int& L, float& A, int & V);

	//V always >0; dltB1V >0: inc, <0: dec; banOnly=true: calc until not-ban; false: calc until ban.
	//time_t getSnap_dltV(time_t dlt, float& A, int& V, int & dltB1V, bool banOnly);

	/* Timing */
	struct lastClk_t
	{
		int minu_last;
		int quote_last_minu;
		//int quote_last_ms;
	};

	lastClk_t reportClk(void) const;

	/* analyze tools */
	void newAtBack(void);
	void newAtFront(void);

};


class stkInfo_lib_t
{
public:
	//access stkInfo by stkName
	void insert(const std::string & stkName, std::shared_ptr<stkInfo_t> info);
	size_t count(const std::string & stkName) const;
	stkInfo_t* get(const std::string & stkName) const;
	// void get(const std::string & stkName, stkInfo_t*& info) const;
	const infK_t* getDay_abs(const std::string & stkName, time_t t) const;

	//access stkInfo by mkCode
	void insert(mktCodeI_t mkCode, std::shared_ptr<stkInfo_t> info);
	size_t count(mktCodeI_t mkCode) const;
	stkInfo_t* get(mktCodeI_t mkCode) const;
	// void get(int mkCode, stkInfo_t*& info) const;
	const infK_t* getDay_abs(mktCodeI_t mkCode, time_t t) const;

	//L1 snap
	//void add_snap(int mkCode, int date, const tdx_snap_t& snap);
	// infSnapQ_t* getSnapQ(int mkCode);

	const stkInfo_t* getMk(const std::string & stkName) const;
	const stkInfo_t* getMk(int mkCode) const;


	// void setBase(time_t now); 
	// void nextBase(time_t now);
	void setBase(time_t dayClk, time_t dayMinuClk);	//apply to all stkInfo
	void nextBase(time_t dayClk, time_t dayMinuClk);	//apply to all stkInfo

	void setBaseDay(time_t dayClk);
	void nextBaseDay(time_t dayClk);

	void setBaseInDay(time_t dayMinuClk);
	void nextBaseInDay(time_t dayMinuClk);

	size_t size(void)const {
		return _lib.size();
	}
	void append(const stkInfo_lib_t& lib);

	//void setSnapDB(bool bToDB, bool bFromDB, const std::string & dbTable, const std::string & dbPath);

	//void snap_toDB(void);

	void doOTF(void);

	struct lastClk_t
	{
		int minu_last_MIN;
		int minu_last_MAX;

		int quote_last_minu_MIN;
		int quote_last_minu_MAX;

		//int quote_last_ms_MIN;
		//int quote_last_ms_MAX;
	};
	lastClk_t reportClk(void) const;

	typedef std::map<mktCodeI_t, std::shared_ptr<stkInfo_t>> _lib_t;
	_lib_t::const_iterator  begin(void) const { return _lib.begin(); };
	_lib_t::const_iterator end(void) const { return _lib.end(); };
private:
	_lib_t _lib;
};

//run thread to read/preload K/basic/info from db/ol.
class thread_data_if_t
{
public:
	thread_data_if_t(int nb_thread = 1);

	~thread_data_if_t() {
		finish();

		for (int i = 0; i < _nb_thread; ++i) {
			thread_join(th_id[i]);
		}

	};

	//block, return false= not ready
	// bool getOL_to_nowLib(const std::set<int>& reqList, const struct OLreq_t& reqInfo);
	// void snap_toDB(void);
    //bToDB is used in getOL_to_nowLib; bFromDB is not used now, but in preload will set SnapQ.fromDB if snap_need
	// void setSnapDB(bool bToDB, bool bFromDB, const std::string& dbPath_toSave, const snapDBconfs_t& sanpDBconfs_toLoad);

	//non-block, return false= not ready
	bool preload(const std::set<std::string>& reqList, const struct stkInfoReq_t& reqInfo);
	bool preloadReady(void);
	bool usePreload(bool append);

	int nowLibFinalDay(void);

	// void getInfo(const std::string & stkName, S4::stkInfo_t*& info);
	// void getInfo(int mkCode, S4::stkInfo_t* & info);

	stkInfo_lib_t* getNowLib(void) {
		return _pNowLib.get();
	}
	
	//int getLastTradedate(void);
	//int getCurrentOLdate(void);

	void reportClk(void) const;

	//void reportOL(void);

	const std::set<std::string>& getNowReqList()const
	{
		return _nowReqList;
	}
	
private:
    int _lastLinkMinuSec;
	std::shared_ptr<TDX::tdxLocal_t> _pTdxLocal;
	// std::shared_ptr < TDX::tdxOL_t> _pTdxOL;

	std::shared_ptr<map_fhps_t> _pFhpsLib;
	std::shared_ptr<map_gbbd_t> _pGbbdLib;

	struct th_arg_t {
		thread_data_if_t* pHost;
		int seq;
	};

	std::vector<thread_id> th_id;
	Mutex _mutex;
	Mutex _mutex_nextLib;
	Mutex _mutex_dbTble;

	std::shared_ptr<stkInfo_lib_t> _pNowLib;
	std::set<std::string> _nowReqList;
	struct stkInfoReq_t _nowReqInfo;

	std::shared_ptr<stkInfo_lib_t> _pNextLib;
	std::set<std::string> _nextReqList;
	struct stkInfoReq_t _nextReqInfo;
	std::set<std::string>::const_iterator _nextProcess;

	std::string _nextDayCondition;
	std::string _nextMinuCondition;

	const int _nb_thread;

	volatile bool _preaload_ready;
	volatile uint64_t _busy;
	volatile uint64_t _doPreload;
	volatile bool _terminate;

	//int _lastTradedate;
	//int _currentOLdate;
	//std::string _currentOLdate_str;

    //struct OLreq_t _OLreq;
    //volatile uint64_t _doOL;
	//volatile bool _OL_ready;
	//std::set<int> _ol_reqList;
	//std::set<int>::const_iterator _next_ol_process;

	//std::map<int, std::shared_ptr<sqlite::DB_t>> _snapDB_list;

	//bool _snap_toDB;
	//bool _snap_fromDB;
	//std::string _snap_dbPath_toSave;
	//glb_conf_ctx_t::snap_t::DB_list_t _snapDBconfs_toLoad;
private:
	void main_run(int seq);

	static THREAD_PROC run_body(void* arg) { //static is necessary.
		struct th_arg_t* Arg = static_cast<struct th_arg_t*>(arg);
		thread_data_if_t * host = Arg->pHost;
		host->main_run(Arg->seq);

		delete Arg;

		return 0;
	}

	void setFQinfo(std::shared_ptr<map_fhps_t> pFhpsLib, std::shared_ptr <map_gbbd_t> pGbbdLib) {
		_pFhpsLib = pFhpsLib;
		_pGbbdLib = pGbbdLib;
	};

	const std::shared_ptr<map_fhps_t> getFhpsLib(void) { 
		return _pFhpsLib; 
	};
	const std::shared_ptr<map_gbbd_t> getGbbdLib(void) { 
		return _pGbbdLib; 
	};

	void loadFQinfo(void)
	{
		std::shared_ptr<map_fhps_t> fhpsLib = std::make_shared<map_fhps_t>();
		std::shared_ptr<map_gbbd_t> gbbdLib = std::make_shared <map_gbbd_t>();
		_pTdxLocal->readGBBQ(*gbbdLib, *fhpsLib);
		setFQinfo(fhpsLib, gbbdLib);
	}

	// void readDBdayK(const std::string & stkName, S4::infKQ_t& dayKQ, int nb, const std::string& condition="");

	bool readTDXdayK(const std::string& mktCode, S4::infKQ_t& dayKQ, int nb_preEnd, int end);
	// void readDBminuK(const std::string & stkName, S4::infKQ_t& minuKQ, int nb, const std::string& condition = "");
	// void readTDXminuK(const std::string & stkName, S4::infKQ_t& minuKQ, int nb_preEnd, int end);

	bool isNextReqListDone(void) {
		//if (_doOL)
		//	return _next_ol_process == _ol_reqList.end();
		//else
			return _nextProcess == _nextReqList.end();
	}

	// bool isTdxOLready(void);

	void getThreadNextRange(std::set<std::string>::const_iterator& bgn, std::set<std::string>::const_iterator& end);
	//void getThreadNextRange(std::set<int>::const_iterator& bgn, std::set<int>::const_iterator& end);

	void loadStkInfo(const std::string & stkName, std::shared_ptr<stkInfo_lib_t> pLib);
	void multiLoadNext(int seq, std::shared_ptr<stkInfo_lib_t> pLib, std::set<std::string>::const_iterator bgn, std::set<std::string>::const_iterator end);

	//void downloadStkInfo(int mkCode, std::shared_ptr<stkInfo_lib_t> pLib);
	//void multiLoadNext(int seq, std::shared_ptr<stkInfo_lib_t> pLib, std::set<int>::const_iterator bgn, std::set<int>::const_iterator end);

	void joinLib(std::shared_ptr<stkInfo_lib_t> pLib);

	void finish(void) { _terminate = true; };

};


}