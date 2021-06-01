#pragma once
#include "data/s4thread_data_if.h"
#include "common/s4logger.h"

#define TDX_DATA
#define DO_CHECK_NON_LOAD


using namespace std;

namespace S4{

#define THREAD_READ_TRUNK 80

CREATE_LOCAL_LOGGER("th_data_if")

//stkInfo_t::stkInfo_t():
//	pDayKQ(NULL),
//	pCycQlib(),
//	pMinuKQ(NULL),
//	name("null")
//{}


stkInfo_t::~stkInfo_t()
{
	//if(pDayKQ!=NULL){
	//    delete pDayKQ;
	//}
	//pDayKQ = NULL;
	
	// if(pCycQlib!=NULL){
	//     delete pCycQlib;
	// }
	// pCycQlib = NULL;

	// if (pMAlib != NULL) {
	// 	delete pMAlib;
	// }
	// pMAlib = NULL;

}

stkInfo_t::stkInfo_t(const std::string&s) :
	pDayKQ(NULL),
	// pCycQlib(NULL),
	pMAlib(NULL),
	pMinuKQ(NULL),
	_name(s),
	_mkCodeInt(mktCodeStr_to_mktCodeInt(s))
	// pSnanpQ(NULL)
{}


void stkInfo_t::setBase(time_t dayClk, time_t dayMinuClk)
{
	setBaseDay(dayClk);
	setBaseInDay(dayMinuClk);
}

void stkInfo_t::setBaseDay(time_t dayClk)
{
	if (pDayKQ != NULL)
		pDayKQ->setBase(dayClk);

	// if (pCycQlib != NULL) {
	// 	for (auto& i : *pCycQlib) {
	// 		i.second->setBase(dayClk);
	// 	}
	// }
	if (pMAlib != NULL) {
		for (auto& i : *pMAlib) {
			i.second->setBase(dayClk);
		}
	}
}

void stkInfo_t::setBaseInDay(time_t dayMinuClk)
{
	if (pMinuKQ != NULL)
		pMinuKQ->setBase(dayMinuClk);

	// if (pSnanpQ != NULL)
	// 	pSnanpQ->setBase(dayMinuClk);
}


void stkInfo_t::nextBase(time_t dayClk, time_t dayMinuClk)
{
	nextBaseDay(dayClk);
	nextBaseInDay(dayMinuClk);
}

void stkInfo_t::nextBaseDay(time_t dayClk)
{
	if (pDayKQ != NULL)
		pDayKQ->nextBase(dayClk);

	// if (pCycQlib != NULL) {
	// 	for (auto& i : *pCycQlib) {
	// 		i.second->nextBase(dayClk);
	// 	}
	// }

	if (pMAlib != NULL) {
		for (auto& i : *pMAlib) {
			i.second->nextBase(dayClk);
		}
	}
}
void stkInfo_t::nextBaseInDay(time_t dayMinuClk)
{
	if (pMinuKQ != NULL)
		pMinuKQ->nextBase(dayMinuClk);

	// if (pSnanpQ != NULL)
	// 	pSnanpQ->nextBase(dayMinuClk);
}

void stkInfo_t::doOTF()
{
	if (pDayKQ != NULL)
		pDayKQ->doOTF();

	//for (auto& i : *pCycQlib) {
	//	i.second->doOTF();
	//}

	for (auto& i : *pMAlib) {
		i.second->doOTF();
	}

	// if (pMinuKQ != NULL)
	// 	pMinuKQ->doOTF();

	// if (pSnanpQ != NULL)
	// 	pSnanpQ->doOTF();
}

const infK_t* stkInfo_t::getDay(int i) const{
	if (pDayKQ == NULL)
		return NULL;
	return pDayKQ->getInfo_ref(i);
}

int stkInfo_t::getDay_max_high(int bgn, int end)
{
	int max_high = 0;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		max_high = _MAX_(max_high, pDayKQ->getInfo_ref(i)->high);
	}
	return max_high;
}

int stkInfo_t::getDay_min_low(int bgn, int end)
{
	int min_low = 0x7fffffff;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		min_low = _MIN_(min_low, pDayKQ->getInfo_ref(i)->low);
	}
	return min_low;
}


int stkInfo_t::getDay_max_avg(int bgn, int end)
{
	int max_high = 0;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		max_high = _MAX_(max_high, pDayKQ->getInfo_ref(i)->prcAvg());
	}
	return max_high;
}

int stkInfo_t::getDay_min_avg(int bgn, int end)
{
	int min_low = 0x7fffffff;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		min_low = _MIN_(min_low, pDayKQ->getInfo_ref(i)->prcAvg());
	}
	return min_low;
}
//
int stkInfo_t::getDay_max_high_fq(int bgn, int end)
{
	int max_high = 0;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		max_high = _MAX_(max_high, pDayKQ->getInfo_ref(i)->high_fq());
	}
	return max_high;
}

int stkInfo_t::getDay_min_low_fq(int bgn, int end)
{
	int min_low = 0x7fffffff;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		min_low = _MIN_(min_low, pDayKQ->getInfo_ref(i)->low_fq());
	}
	return min_low;
}


int stkInfo_t::getDay_max_avg_fq(int bgn, int end)
{
	int max_high = 0;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		max_high = _MAX_(max_high, pDayKQ->getInfo_ref(i)->prcAvg_fq());
	}
	return max_high;
}

int stkInfo_t::getDay_min_avg_fq(int bgn, int end)
{
	int min_low = 0x7fffffff;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		min_low = _MIN_(min_low, pDayKQ->getInfo_ref(i)->prcAvg_fq());
	}
	return min_low;
}
//
// cyc_t* stkInfo_t::getCyc(int scope, int i) {
// 	if (pCycQlib!=NULL && pCycQlib->count(scope) == 0)
// 		return NULL;
// 	return pCycQlib->at(scope)->getInfo_ref(i);
// };

// //
// CYCQ_t* stkInfo_t::getCycQ(int scope) {
// 	if (pCycQlib != NULL && pCycQlib->count(scope) == 0)
// 		return NULL;
// 	return pCycQlib->at(scope).get();
// };

// int stkInfo_t::getCyc_max_avg(int scope, int bgn, int end)
// {
// 	if (pCycQlib->count(scope) == 0)
// 		return -1;

// 	int max_avg = 0;
// 	for (int i = bgn; i <= end; ++i) {
// 		if (pCycQlib->at(scope)->getInfo_ref(i) != NULL)
// 			max_avg = _MAX_(max_avg, pCycQlib->at(scope)->getInfo_ref(i)->_status.avgPrice);
// 		else
// 			break;
// 	}
// 	return max_avg;
// }

int stkInfo_t::getDay_max_close(int bgn, int end)
{
	int max_close= 0;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		max_close = _MAX_(max_close, pDayKQ->getInfo_ref(i)->close);
	}
	return max_close;
}

int stkInfo_t::getDay_max_close_fq(int bgn, int end)
{
	int max_close = 0;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		max_close = _MAX_(max_close, pDayKQ->getInfo_ref(i)->close_fq());
	}
	return max_close;
}


vol_share_t stkInfo_t::getDay_max_vol(int bgn, int end)
{
	vol_share_t max_vol = 0;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		max_vol = _MAX_(max_vol, pDayKQ->getInfo_ref(i)->volume);
	}
	return max_vol;
}

vol_share_t stkInfo_t::getDay_min_vol(int bgn, int end)
{
	vol_share_t min_vol = 0;
	for (int i = bgn; i <= end; ++i) {
		if (pDayKQ->getInfo_ref(i) == NULL) break;
		min_vol = _MIN_(min_vol, pDayKQ->getInfo_ref(i)->volume);
	}
	return min_vol;
}
//
const ma_t* stkInfo_t::getMA(int scope, int i) const {
	if (pMAlib->count(scope) == 0)
		return NULL;
	return pMAlib->at(scope)->getInfo_ref(i);
}

int stkInfo_t::getMA_max(int scope, int bgn, int end)
{
	if (pMAlib->count(scope) == 0)
		return -1;

	int max_avg = 0;
	for (int i = bgn; i <= end; ++i) {
		if (pMAlib->at(scope)->getInfo_ref(i) != NULL)
			max_avg = _MAX_(max_avg, pMAlib->at(scope)->getInfo_ref(i)->C());
		else
			break;
	}
	return max_avg;
}

int stkInfo_t::getMA_min(int scope, int bgn, int end)
{
	if (pMAlib->count(scope) == 0)
		return -1;

	int min_avg = 0x7fffffff;
	for (int i = bgn; i <= end; ++i) {
		if (pMAlib->at(scope)->getInfo_ref(i) != NULL)
			min_avg = _MIN_(min_avg, pMAlib->at(scope)->getInfo_ref(i)->C());
		else
			break;
	}
	return min_avg;
}

int stkInfo_t::dayNb_Cross(int price, int bgn, int len, int iGap) const
{
	if (pDayKQ == NULL)
		return 0;
	if (pDayKQ->getInfo_ref(bgn + len) == NULL)
		return 0;

	int upNb = 0;
	for (int i = bgn; i < bgn + len; ++i) {
		if(pDayKQ->getInfo_ref(i) == NULL)
			break;
		if (pDayKQ->getInfo_ref(i)->close_fq() > price)
			upNb++;
	}

	int nb = 0;
	for (int i = bgn+iGap; i <= bgn+len-iGap; ++i) {
		if (pDayKQ->getInfo_ref(i - iGap) == NULL || pDayKQ->getInfo_ref(i) == NULL || pDayKQ->getInfo_ref(i + iGap) == NULL)
			break;
	
		if (pDayKQ->getInfo_ref(i - iGap)->close_fq() < price &&
			pDayKQ->getInfo_ref(i)->high_fq() > price &&
			pDayKQ->getInfo_ref(i + iGap)->close_fq() < price) {
			nb++;
		}
	}
	if (nb*2 >= upNb)
		return nb;
	else
		return 0;
}

const infK_t* stkInfo_t::getMinu(int i) const
{
	if (pMinuKQ == NULL)
		return NULL;
	return pMinuKQ->getInfo_ref(i);
}
/*
snap
*/

//
void stkInfo_t::addInfo(stkInfo_t& newInfo)
{
	if (this->name() != newInfo.name()) {
		LCL_ERR("Illegal to add stkInfo of {} to {}", this->name(), newInfo.name());
		return;
	}
	//TODO: copy to NULL, use share_ptr
	if (this->pDayKQ == NULL || this->pDayKQ->size()==0) {
		this->pDayKQ = newInfo.pDayKQ; //share_ptr
	}
	else if (this->pDayKQ != NULL && newInfo.pDayKQ != NULL) {
		this->pDayKQ->addQ(*newInfo.pDayKQ);
	}

	if (this->pMinuKQ == NULL || this->pMinuKQ->size()==0) {
		this->pMinuKQ = newInfo.pMinuKQ; //share_ptr
	}
	else if (this->pMinuKQ != NULL && newInfo.pMinuKQ != NULL) {
		this->pMinuKQ->addQ(*newInfo.pMinuKQ);
	}

	// if (this->pSnanpQ == NULL || this->pSnanpQ->size()==0) {
	// 	this->pSnanpQ = newInfo.pSnanpQ; //share_ptr
	// }
	// else if (this->pSnanpQ != NULL && newInfo.pSnanpQ != NULL) {
	// 	this->pSnanpQ->addQ(*newInfo.pSnanpQ, SNAP_SIZE_LMT_NB);
	// }

	// if ((this->pCycQlib == NULL || this->pCycQlib->size()==0) && newInfo.pCycQlib != NULL) {
    //     if(this->pCycQlib == NULL)
    //         this->pCycQlib = new std::map<int, std::shared_ptr<CYCQ_t>>;
	// 	for (auto& p : *newInfo.pCycQlib) {
	// 		this->pCycQlib->insert(p);
	// 	}
	// }
	// else if (this->pCycQlib != NULL && newInfo.pCycQlib != NULL) {
	// 	for (auto& p : *this->pCycQlib) {
	// 		if (newInfo.pCycQlib->count(p.first) > 0) {
	// 			p.second->addQ(*newInfo.pCycQlib->at(p.first));
	// 		}
	// 	}
	// }

	if ((this->pMAlib == NULL || this->pMAlib->size()==0) && newInfo.pMAlib != NULL) {
        if(this->pMAlib == NULL)
		    this->pMAlib = make_shared<std::map<int, std::shared_ptr<maQ_t>>>();
		for (auto& p : *newInfo.pMAlib) {
			this->pMAlib->insert(p);
		}
	}
	else if (this->pMAlib != NULL && newInfo.pMAlib != NULL) {
		for (auto& p : *this->pMAlib) {
			if (newInfo.pMAlib->count(p.first) > 0) {
				p.second->addQ(*newInfo.pMAlib->at(p.first));
			}
		}
	}
}

stkInfo_t::lastClk_t stkInfo_t::reportClk(void) const
{
	lastClk_t lastClk = { 0,0 };
	//if (pMinuKQ == NULL || pMinuKQ->size()==0 || 
	//	pSnanpQ==NULL || pSnanpQ->size()==0) {
	//	return move(lastClk);
	//}
	if(pMinuKQ != NULL && pMinuKQ->size() != 0)
		lastClk.minu_last = (*pMinuKQ->rbegin())->_MinmuSec;

	// if(pSnanpQ != NULL && pSnanpQ->size() != 0)
	// 	lastClk.quote_last_minu = (*pSnanpQ->rbegin())->_MinmuSec;

	//lastClk.quote_last_ms = (*pSnanpQ->rbegin())->ms;
	return move(lastClk);
}

void stkInfo_t::newAtBack(void)
{
	if (this->pDayKQ != NULL && this->pDayKQ->size() != 0) {
		this->pDayKQ->newAtBack(); //share_ptr
	}

	if (this->pMinuKQ != NULL && this->pMinuKQ->size() != 0) {
		this->pMinuKQ->newAtBack(); //share_ptr
	}

	// if (this->pSnanpQ != NULL && this->pSnanpQ->size()==0) {
	// 	this->pSnanpQ->newAtBack();
	// }


	// 	for (auto& p : *this->pCycQlib) {
	// 		p.second->newAtBack();
	// 	}

	for (auto& p : *this->pMAlib) {
		p.second->newAtBack();
	}
}
void stkInfo_t::newAtFront(void)
{
	if (this->pDayKQ != NULL && this->pDayKQ->size() != 0) {
		this->pDayKQ->newAtFront(); //share_ptr
	}

	if (this->pMinuKQ != NULL && this->pMinuKQ->size() != 0) {
		this->pMinuKQ->newAtFront(); //share_ptr
	}

	// if (this->pSnanpQ != NULL && this->pSnanpQ->size()!=0) {
	// 	this->pSnanpQ->newAtFront();
	// }


	// 	for (auto& p : *this->pCycQlib) {
	// 		p.second->newAtFront();
	// 	}

	for (auto& p : *this->pMAlib) {
		p.second->newAtFront();
	}
}

/************************************************************************/
/* stkInfo_lib_t                                                        */
/************************************************************************/
size_t stkInfo_lib_t::count(const std::string & stkName) const
{
	int mkCode = mktCodeStr_to_mktCodeInt(stkName);
    return _lib.count(mkCode);
}

void stkInfo_lib_t::insert(const std::string & stkName, std::shared_ptr<stkInfo_t> info)
{
	int mkCode = mktCodeStr_to_mktCodeInt(stkName);
    _lib.insert(std::pair<int, std::shared_ptr<stkInfo_t>>(mkCode, info));
}

stkInfo_t* stkInfo_lib_t::get(const std::string & stkName) const
{
	int mkCode = mktCodeStr_to_mktCodeInt(stkName);
	if (_lib.count(mkCode) == 0) {
#ifndef DO_CHECK_NON_LOAD
		return nullptr;
#endif // !DO_CHECK_NON_LOAD
		std::cout << "Try to getInfo of none-loading stk:" + stkName << endl;
		throw ThreadDataIfError("Try to getInfo of non-loading stk:" + stkName);
	}
	return _lib.find(mkCode)->second.get();
}

//"sz399001", //深圳成指
//"sz399005", //中小扳指
//"sz399006"  //创业板
const stkInfo_t* stkInfo_lib_t::getMk(const std::string & stkName) const
{
	int mkCode = mktCodeStr_to_mktCodeInt(stkName);
	return getMk(mkCode);
}

const stkInfo_t* stkInfo_lib_t::getMk(int mkCode) const
{
	if (!isStk(mkCode))
		return NULL;

	if (isCYmkt(mkCode)) {
		return get(CODE_INT_STK_INDEX_CY);
	}
	else if (isSZmkt(mkCode)) {	//SZ include CY
		return get(CODE_INT_STK_INDEX_SZ);
	}
	else if (isSHmkt(mkCode)) {
		return get(CODE_INT_STK_INDEX_SH);
	}

	return NULL;
}


size_t stkInfo_lib_t::count(mktCodeI_t mkCode) const
{
	return _lib.count(mkCode);
}

void stkInfo_lib_t::insert(mktCodeI_t mkCode, std::shared_ptr<stkInfo_t> info)
{
	_lib.insert(std::pair<mktCodeI_t, std::shared_ptr<stkInfo_t>>(mkCode, info));
}

stkInfo_t* stkInfo_lib_t::get(mktCodeI_t mkCode) const
{
	if (_lib.count(mkCode) == 0) {
#ifndef DO_CHECK_NON_LOAD
		return nullptr;
#endif // !DO_CHECK_NON_LOAD
		std::cout << "Try to getInfo of none-loading stk:" + mktCodeInt_to_mktCodeStr(mkCode) << endl;
		throw ThreadDataIfError("Try to getInfo of none-loading stk:" + mktCodeInt_to_mktCodeStr(mkCode));
	}
	return _lib.find(mkCode)->second.get();
}

void stkInfo_lib_t::setBase(time_t dayClk, time_t dayMinuClk)
{
	for (_lib_t::iterator i = _lib.begin(); i != _lib.end(); ++i) {
		i->second->setBase(dayClk, dayMinuClk);
	}
}

void stkInfo_lib_t::nextBase(time_t dayClk, time_t dayMinuClk)
{
	for (_lib_t::iterator i = _lib.begin(); i != _lib.end(); ++i) {
		i->second->nextBase(dayClk, dayMinuClk);
	}
}

void stkInfo_lib_t::setBaseDay(time_t dayClk)
{
	for (_lib_t::iterator i = _lib.begin(); i != _lib.end(); ++i) {
		i->second->setBaseDay(dayClk);
	}
}
void stkInfo_lib_t::nextBaseDay(time_t dayClk)
{
	for (_lib_t::iterator i = _lib.begin(); i != _lib.end(); ++i) {
		i->second->nextBaseDay(dayClk);
	}
}

void stkInfo_lib_t::setBaseInDay(time_t dayMinuClk)
{
	for (_lib_t::iterator i = _lib.begin(); i != _lib.end(); ++i) {
		i->second->setBaseInDay(dayMinuClk);
	}
}
void stkInfo_lib_t::nextBaseInDay(time_t dayMinuClk)
{
	for (_lib_t::iterator i = _lib.begin(); i != _lib.end(); ++i) {
		i->second->nextBaseInDay(dayMinuClk);
	}
}



void stkInfo_lib_t::append(const stkInfo_lib_t& lib)
{
	for (_lib_t::const_iterator i = lib.begin(); i != lib.end(); ++i) {
		if (_lib.count(i->first) == 0) {
			_lib[i->first] = i->second;
		}
		else {
			_lib[i->first]->addInfo(*i->second);
		}
	}
}

// void stkInfo_lib_t::setSnapDB(bool bToDB, bool bFromDB, const std::string & dbTable, const std::string & dbPath);

// void stkInfo_lib_t::snap_toDB(void);

void stkInfo_lib_t::doOTF(void) {
	for (_lib_t::iterator i = _lib.begin(); i != _lib.end(); ++i) {
		i->second->doOTF();
	}
}
//stkInfo_lib_t::~stkInfo_lib_t();
	
// void stkInfo_lib_t::get(const std::string & stkName, S4::stkInfo_t*& info ) const;

const infK_t* stkInfo_lib_t::getDay_abs(const std::string & stkName, time_t t) const{
	const stkInfo_t* pStk = get(stkName);
	if (pStk != NULL)
		return pStk->getDay_abs(t);
	return NULL;
}

const infK_t* stkInfo_lib_t::getDay_abs(mktCodeI_t mkCode, time_t t) const{
	const stkInfo_t* pStk = get(mkCode);
	if (pStk != NULL)
		return pStk->getDay_abs(t);
	return NULL;
}

// void stkInfo_lib_t::add_quote(int mkCode, int date, const stk_tdx_quote_t& quote);

// infSnapQ_t* stkInfo_lib_t::getSnapQ(int mkCode);

stkInfo_lib_t::lastClk_t stkInfo_lib_t::reportClk(void) const
{
	lastClk_t rept = { 99999999, 0, 99999999, 0 };
	for (auto& i : _lib) {
		stkInfo_t::lastClk_t cur_lastClk = i.second->reportClk();
		if (cur_lastClk.minu_last != 0) {
			if (rept.minu_last_MAX < cur_lastClk.minu_last)rept.minu_last_MAX = cur_lastClk.minu_last;
			if (rept.minu_last_MIN > cur_lastClk.minu_last)rept.minu_last_MIN = cur_lastClk.minu_last;
		}
		if (cur_lastClk.quote_last_minu != 0) {
			if (rept.quote_last_minu_MAX < cur_lastClk.quote_last_minu)rept.quote_last_minu_MAX = cur_lastClk.quote_last_minu;
			if (rept.quote_last_minu_MIN > cur_lastClk.quote_last_minu)rept.quote_last_minu_MIN = cur_lastClk.quote_last_minu;
		}
		//if (rept.quote_last_ms_MAX < cur_lastClk.quote_last_ms)rept.quote_last_ms_MAX = cur_lastClk.quote_last_ms;
		//if (rept.quote_last_ms_MIN > cur_lastClk.quote_last_ms)rept.quote_last_ms_MIN = cur_lastClk.quote_last_ms;
	}
	return move(rept);
}
/************************************************************************/
/* thread_data_if_t                                                     */
/************************************************************************/
thread_data_if_t::thread_data_if_t(int nb_thread) :
    _lastLinkMinuSec(0),
	// _pTdxOL(NULL),
	_nb_thread(nb_thread),
	_preaload_ready(false),
	_busy(0),
	_doPreload(0),
	_terminate(false)
	//_lastTradedate(0),
	//_currentOLdate(0),
	//_doOL(0),
	//_OL_ready(false),
	//_snap_toDB(false)
{
	filesystem::path tdx_root(glb_conf::pInstance()->tdx().root);
#ifdef TDX_DATA
	if (!std::filesystem::exists(tdx_root) || !is_directory(tdx_root)) {
		LCL_FATAL("no such folder: {:}", tdx_root.string());
	}
	_pTdxLocal = make_shared<TDX::tdxLocal_t>(tdx_root.string());
#endif // TDX_DATA


	loadFQinfo();

	th_id.resize(_nb_thread);
	for (int i = 0; i < _nb_thread; ++i) {
		struct th_arg_t* pArg = new struct th_arg_t;
		pArg->pHost = this;
		pArg->seq = i;
		if (!thread_spawn(&run_body, pArg, th_id[i])) {
			//report error
			throw ThreadDataIfError("Create thread of thread_data_if_t failed!");
		}
	}
	LCL_INFO("thread_data_if init successed, thread nb={}", nb_thread);
}

// bool thread_data_if_t::isTdxOLready(void);

// void thread_data_if_t::setSnapDB(bool bToDB, bool bFromDB, const std::string& dbPath_toSave, const snapDBconfs_t& snapDBconfs_toLoad) ;

// bool thread_data_if_t::getOL_to_nowLib(const std::set<int>& reqList, const struct OLreq_t& reqInfo);


bool thread_data_if_t::preload(const std::set<std::string>& reqList, const struct stkInfoReq_t& reqInfo)
{
	if (_busy>0)
		return false;//no thread is ready for preload

	Locker lock(_mutex);
	//if (_pNextLib != NULL)
	//not delete here!	delete _pNextLib;
	_pNextLib = std::make_shared<stkInfo_lib_t>();
	LCL_INFO("built new _pNextLib... day({:d} days pre {:d})  minu({:d} minus pre {:d}, need={:d})  snap(need={})", 
		reqInfo.nbDay_preEndDate, reqInfo.endDate, reqInfo.nbMinu_preEndMinuDate, reqInfo.endDate, reqInfo.needMinu,
		reqInfo.needSnap);

	_nextReqInfo = reqInfo;
	_nextReqList = reqList;

	for (int i = 0; i < _nb_thread; ++i)
		_doPreload |= (1ULL << i);
	_preaload_ready = false;
    _nextProcess = _nextReqList.begin();

	char p[1024];
#ifdef _MSC_VER
	sprintf_s(p, sizeof(p), "where date <= %u order by date desc limit %u", _nextReqInfo.endDate, reqInfo.nbDay_preEndDate);
#else
	snprintf(p, sizeof(p), "where date <= %u order by date desc limit %u", _nextReqInfo.endDate, reqInfo.nbDay_preEndDate);
#endif
	_nextDayCondition = std::string(p);

#ifdef _MSC_VER
	sprintf_s(p, sizeof(p), "where time <= %llu order by time desc limit %u", date_to_utc(_nextReqInfo.endMinuDate) + 3600 * 23, reqInfo.nbMinu_preEndMinuDate);
#else
	snprintf(p, sizeof(p), "where time <= %llu order by time desc limit %u", date_to_utc(_nextReqInfo.endMinuDate) + 3600 * 23, reqInfo.nbMinu_preEndMinuDate);
#endif
	_nextMinuCondition = std::string(p);

	return true;
}
bool thread_data_if_t::preloadReady(void)
{
	return _preaload_ready;
}
bool thread_data_if_t::usePreload(bool append)
{
	if (_preaload_ready) {
		Locker lock(_mutex);
		if (!append || !_pNowLib || _nowReqInfo != _nextReqInfo) {//replace
			_pNowLib = _pNextLib;
			_nowReqList = _nextReqList;
			_nowReqInfo = _nextReqInfo;
			LCL_INFO("use preload replace!");
		}
		else {
			_pNowLib->append(*_pNextLib);
			_nowReqList.insert(_nextReqList.begin(), _nextReqList.end());
			LCL_INFO("use preload append!");
		}
		return true;
	}
	LCL_INFO("Cannot use preload before ready!");
	return false;
}
int thread_data_if_t::nowLibFinalDay(void)
{
	if (_pNowLib == NULL) {
		throw ThreadDataIfError("nowLibFinalDay(): nowLib not ready!");
	}

	return _nowReqInfo.endDate;
}

// int thread_data_if_t::getLastTradedate(void);

// int thread_data_if_t::getCurrentOLdate(void);

void thread_data_if_t::reportClk(void) const
{
	if (_pNowLib == NULL) {
		throw ThreadDataIfError("reportClk(): nowLib not ready!");
	}

	stkInfo_lib_t::lastClk_t rpt = _pNowLib->reportClk();

	LCL_INFO("current MINU in [{}, {}];  QUOTE in minu[{}, {}]",
		rpt.minu_last_MIN, rpt.minu_last_MAX,
		rpt.quote_last_minu_MIN, rpt.quote_last_minu_MAX
		//rpt.quote_last_ms_MIN, rpt.quote_last_ms_MAX
		);
}
// void thread_data_if_t::reportOL(void) ;

void thread_data_if_t::main_run(int seq)
{
	bool doPreload = false;
	//bool doOL = false;
	while (!_terminate) {
		if ((_doPreload & (1ULL << seq)) > 0) {
			doPreload = true;

			Locker lock(_mutex);
			_busy |= (1ULL << seq);
		}
		//else if ((_doOL & (1ULL << seq)) > 0) {
		//	doOL = true;

		//	Locker lock(_mutex);
		//	_busy |= (1ULL << seq);
		//}

		//if (doPreload || doOL) {
			//do
			if (doPreload) {
				std::set<std::string>::const_iterator bgn, end;
				while (!isNextReqListDone()) {
					std::shared_ptr<stkInfo_lib_t> pLib = std::make_shared<stkInfo_lib_t>();
					getThreadNextRange(bgn, end);

					multiLoadNext(seq, pLib, bgn, end);

					joinLib(pLib);
				}
			}
			//else {
			//	std::set<int>::const_iterator bgn, end;
			//	while (!isNextReqListDone()) {
			//		std::shared_ptr<stkInfo_lib_t> pLib = std::make_shared<stkInfo_lib_t>();
			//		getThreadNextRange(bgn, end);

			//		multiLoadNext(seq, pLib, bgn, end);

			//		joinLib(pLib);
			//	}
			//}

			Locker lock(_mutex);
			_busy &= ~(1 << seq);
			if (doPreload) {
				_doPreload &= ~(1 << seq);
				if (_busy == 0)
					_preaload_ready = true;

				LCL_TRAC("thSeq preload{0:d} done, busy={1:x}, preload done={2:d} at {3:d} ms!", seq, _busy, _preaload_ready, nowTime_ms());
			}
			//else {
			//	_doOL &= ~(1 << seq);
			//	if (_busy == 0)
			//		_OL_ready = true;
			//	//LCL_INFO("thSeq OL{0:d} done, busy={1:x}, preload done={2:d} at {3:d} ms!", seq, _busy, _preaload_ready, nowTime_ms());
			//}
			doPreload = false;
			//doOL = false;
		//}
		process_sleep(0.001);	//s
	}
	LCL_INFO("thread({:d}) data if terminated!", th_id[seq]);
}

bool thread_data_if_t::readTDXdayK(const std::string& mktCode, S4::infKQ_t& dayKQ, int nb_preEnd, int end)
{
	vec_dayK_t dayKque_r;
	if (!_pTdxLocal->readDayK_nb(mktCode, end, nb_preEnd, dayKque_r)) {
		return false;
	}
	dayKQ.add_vec_dayK(dayKque_r);
	return true;
}

void thread_data_if_t::loadStkInfo(const std::string & mktCode, std::shared_ptr<stkInfo_lib_t> pLib)
{
	int mkCode = mktCodeStr_to_mktCodeInt(mktCode);

	//if (pLib->count(mkCode) != 0) {
	//	return;
	//}
	assert(pLib->count(mkCode) == 0);

	//const struct basicDB_t::basicOfCode_t* pBasic = isStk(mktCode)?s3global::g_basicDB.getBasic(mktCodeInt_to_pureCodeInt(mkCode)):NULL;

	std::shared_ptr<stkInfo_t> info = std::make_shared<stkInfo_t>(mktCode);

	try {
		//DayK
		info->pDayKQ = make_shared<infKQ_t>();

#ifndef TDX_DATA
		readDBdayK(mktCode, *info->pDayKQ, _nextReqInfo.nbDay_preEndDate, _nextDayCondition);
#else
		if (!readTDXdayK(mktCode, *info->pDayKQ, _nextReqInfo.nbDay_preEndDate, _nextReqInfo.endDate)) {
			return;
		}
#endif
		pLib->insert(mkCode, info);	//only insert existing code
		if ((*info->pDayKQ).size() == 0)
			return;

		//DayK FQ
		info->pDayKQ->calcFQ(getGbbdLib()->getStk(mkCode), getFhpsLib()->getStk(mkCode));

		//dayK_CYC
		// info->pCycQlib = make_shared<std::map<int, shared_ptr <CYCQ_t>>>();
		// for (size_t i = 0; i < _nextReqInfo.cyc_scope_list.size(); ++i) {
		// 	int scope = _nextReqInfo.cyc_scope_list[i];
		// 	if (info->pCycQlib->count(scope) == 0) {
		// 		info->pCycQlib->insert(std::pair<int, shared_ptr <CYCQ_t>>(scope, make_shared<CYCQ_t>(*info->pDayKQ, scope)));
		// 		//info->pCycQlib->at(scope)->addK(*info->pDayKQ);
		// 	}
		// }

		//dayK-MA
		info->pMAlib = make_shared<std::map<int, shared_ptr <maQ_t>>>();
		for (size_t i = 0; i < _nextReqInfo.ma_scope_list.size(); ++i) {
			int scope = _nextReqInfo.ma_scope_list[i];
			if (info->pMAlib->count(scope) == 0) {
				info->pMAlib->insert(std::pair<int, shared_ptr<maQ_t>>(scope, make_shared<maQ_t>(*info->pDayKQ, scope)));
				//info->pCycQlib->at(scope)->addK(*info->pDayKQ);
			}
		}

// 		//minuK
// 		if (_nextReqInfo.needMinu) {
// 			info->pMinuKQ = make_shared<infKQ_t>();
// #ifndef TDX_DATA
// 			readDBminuK(mktCode, *info->pMinuKQ, _nextReqInfo.nbMinu_preEndMinuDate, _nextMinuCondition);
// #else
// 			readTDXminuK(mktCode, *info->pMinuKQ, _nextReqInfo.nbMinu_preEndMinuDate, _nextReqInfo.endMinuDate);
// #endif
// 		}

		// //snapshot

	}
	catch (std::exception& e) {
		LCL_ERR("{} load info error during [nb_DayPre={}, endDate={}]. Info: {}", mktCode, _nextReqInfo.nbDay_preEndDate, _nextReqInfo.endDate, e.what());
		throw e;
	}
}

void thread_data_if_t::getThreadNextRange(std::set<std::string>::const_iterator& bgn, std::set<std::string>::const_iterator& end)
{
	if (isNextReqListDone()) {
		bgn = _nextReqList.end();
		end = _nextReqList.end();
	}
	Locker l(_mutex_nextLib);
	bgn = _nextProcess;
	for (int i = 0;
		i < _MIN_(THREAD_READ_TRUNK, _nextReqList.size() / _nb_thread + 1) && _nextProcess != _nextReqList.end();
		_nextProcess++, i++
		);
	//_nextProcess += THREAD_READ_TRUNK;
	end = _nextProcess;
}

//void thread_data_if_t::getThreadNextRange(std::set<int>::const_iterator& bgn, std::set<int>::const_iterator& end);

void thread_data_if_t::multiLoadNext(int seq, std::shared_ptr<stkInfo_lib_t> pLib, std::set<std::string>::const_iterator bgn, std::set<std::string>::const_iterator end)
{
	for (std::set<std::string>::const_iterator i = bgn; i != end; ++i)
	{
		loadStkInfo(*i, pLib);
	}
}

//void thread_data_if_t::multiLoadNext(int seq, std::shared_ptr<stkInfo_lib_t> pLib, std::set<int>::const_iterator bgn, std::set<int>::const_iterator end);


// void thread_data_if_t::downloadStkInfo(int mkCode, std::shared_ptr<stkInfo_lib_t> pLib);

//
//void thread_data_if_t::multiLoadNext(int seq, std::shared_ptr<stkInfo_lib_t> pLib, std::set<int>::const_iterator bgn, std::set<int>::const_iterator end);

void thread_data_if_t::joinLib(std::shared_ptr<stkInfo_lib_t> pLib) 
{
	Locker l(_mutex_nextLib);
	//if (_doOL) {
	//	_pNowLib->append(*pLib);
	//}
	//else {
		_pNextLib->append(*pLib);
	//}
}


}
