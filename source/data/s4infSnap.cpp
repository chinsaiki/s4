#include "data/s4infSnap.h"
#include "data/s4infSnap_tableIO.h"

using namespace std;

namespace S4 {
infSnapQ_t::~infSnapQ_t() {
	toDB();
}

void infSnapQ_t::dropOld(size_t nb) {
	{
		if (nb >= size()) {
			toDB();
			clear();
			return;
		}

		if (!isNewAtBack())	//keep order to input
			doSwap();

		std::vector<std::shared_ptr<infSnap_t>> newQ(begin() + nb, end());
		toDB();
		clear();

		for (auto i : newQ)
			push_back(i);

		if (!isNewAtBack())	//keep order to before
			doSwap();
	}
}

void infSnapQ_t::use_lcl_time(void)
{
	infQ_t::iterator b;
	for (auto i = begin(); i != end(); ++i) {
		if ((*i)->lcl_time != 0) {

			(*i)->_time = (*i)->lcl_time;
			if (i != begin() && (*i)->_time == (*b)->_time) {//to avid same time
				(*i)->_time++;
			}

			(*i)->_date = utc_to_date((*i)->_time, &((*i)->_MinmuSec));
		}
		b = i;
	}

	//for (auto snap : *this) {
	//	if (snap->lcl_time == 0)
	//		continue;

	//	snap->_time = snap->lcl_time;
	//	snap->_date = utc_to_date(snap->_time, &(snap->_MinmuSec));
	//}
}
void infSnapQ_t::addQ(const infQ_t<infSnap_t>& Q, size_t lmt_size)	//TODO: what about base?
{
	addQ(Q);
	if (lmt_size * 2 < size())//cut to half
		dropOld(size() - lmt_size);
}

void infSnapQ_t::addQ(const S4::infQ_t<infSnap_t>& Q)
{
	//resize(size() + K.size());
	if (!isNewAtBack())	//keep order to input
		doSwap();
	for (infQ_t<infSnap_t>::const_iterator i = Q.begin(); i != Q.end(); ++i)
	{
		if (size() != 0 && (*i)->_date == back()->_date && (*i)->active1 != 0 && (*i)->active1 < back()->active1)
			continue;

		if (size() == 0 || (*i)->_time > back()->_time || ((*i)->_time == back()->_time && (*i)->active1 > back()->active1))	//keep back is new
			push_back(*i);
	}
	if (!isNewAtBack())	//keep order to before
		doSwap();
};

std::vector<time_t> infSnapQ_t::pullTicks(void)
{
	std::vector<time_t> ret;
	if (!isNewAtBack())	//keep order to input
		doSwap();
	for (auto snap : *this) {
		ret.push_back(snap->_time);
	}
	if (!isNewAtBack())	//keep order to before
		doSwap();
	return move(ret);
}


infSnap_t* infSnapQ_t::getLastInfo(time_t clk)
{
	if (size() == 0) {
		return NULL;
	}

	if (isOTF()) {
		if (back()->_time + lastInfoMaxGap > clk || back()->lcl_time + lastInfoMaxGap > clk)
			return back().get();
		else
			return NULL;
	}

	if (!isNewAtBack())
		newAtBack();

	if (back()->_time < clk) {
		if (back()->_time + lastInfoMaxGap > clk)
			return back().get();
		else
			return NULL;
	}

	if ((*this)[0]->_time > clk)
		return NULL;

	int low = 0, high = (int)size(), mid;
	while (low <= high) {
		mid = low + (high - low) / 2;
		if (clk == (*this)[mid]->_time) {
			return (*this)[mid].get();
		}
		else if ((*this)[mid]->_time > clk) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
	if ((*this)[high]->_time + lastInfoMaxGap > clk)
		return (*this)[high].get();
	else
		return NULL;
}


infSnap_t* infSnapQ_t::getLastInfo(time_t clk, int ref)
{
    int ref_abs(ref);
	if (size() == 0) {
		return NULL;
	}

    if(ref<0)
        ref_abs = -ref;

	if (isOTF()) {
		if (back()->_time + lastInfoMaxGap > clk && ref_abs<size())
			return (*this)[size()-ref_abs-1].get();
		else
			return NULL;
	}

	if (!isNewAtBack())
		newAtBack();

	if (back()->_time < clk) {
		if (back()->_time + lastInfoMaxGap > clk && ref_abs<size())
			return (*this)[size()-ref_abs-1].get();
		else
			return NULL;
	}

	if ((*this)[0]->_time > clk)
		return NULL;

	int low = 0, high = (int)size(), mid;
	while (low <= high) {
		mid = low + (high - low) / 2;
		if (clk == (*this)[mid]->_time) {
            if(ref_abs<=mid)
			    return (*this)[mid-ref_abs].get();
            else
		        return NULL;
		}
		else if ((*this)[mid]->_time > clk) {
			high = mid - 1;
		}
		else {
			low = mid + 1;
		}
	}
	if ((*this)[high]->_time + lastInfoMaxGap > clk && ref_abs<=high)
		return (*this)[high-ref_abs].get();
	else
		return NULL;
}

/********************/
// db implement
void infSnapQ_t::toDB() {
	if (!_toDB || size()==0) return;

	sqlite::DB_t db(_dbPath);
	if (!_pTableIO){
		_pTableIO = std::make_shared<sqlite::s4infSnap_tableIO>();
	}
	db.to_table<std::shared_ptr<infSnap_t>>(_pTableIO.get(), _dbTbleName, *this, false);

}

void infSnapQ_t::fromDB(int lmt_Qsize) {
	if (!_fromDB) return;
	string condition(" WHERE minuSec >= 91400 and minuSec <= 150100");
	_read_order_asc = true;
	if (lmt_Qsize != 0) {
		condition = " order by time desc limit " + to_string(lmt_Qsize);
		_read_order_asc = false;
	}
	//_loadDBQ.clear();
	sqlite::DB_t db(_dbPath, SQLite::OPEN_READONLY);
	if (!_pTableIO){
		_pTableIO = std::make_shared<sqlite::s4infSnap_tableIO>();
	}
	db.read_table<std::shared_ptr<infSnap_t>>(_pTableIO.get(), _dbTbleName, *this, condition);

	if (!_read_order_asc)
		doSwap();
	//for (std::map<uint64_t, std::shared_ptr<infSnap_t>>::const_iterator i = _loadDBQ.begin();
	//	i != _loadDBQ.end(); ++i)
	//{
	//	push_back(i->second);
	//}
}

void infSnapQ_t::set_dbTbl_name(const std::string& name)
{
	_dbTbleName = name;
	if (!_pTableIO){
		_pTableIO = std::make_shared<sqlite::s4infSnap_tableIO>();
	}
}

void snapVolAna(const infSnap_t& lastSnap, const infSnap_t& nowSnap, snapVolAna_t& volAna)
{
	int lastVol, nowVol;

	volAna.curAskVolDec = 0;
	volAna.askDecAvgPrice = 0;
	volAna.askDecAvgPriceDiff = 0;
	int askMinPrice = lastSnap.ask1;
	if (askMinPrice == 0) {	//it was upBan, so no askVol traded
	}
	else {
		int askMaxPrice = _MIN_(lastSnap.maxValidAsk(), nowSnap.maxValidAsk());
		if (askMaxPrice == 0) {// nowSnap is upBan
			askMaxPrice = lastSnap.maxValidAsk();//lastSnap is not upBan (always has ask here).
		}

		for (int i = askMinPrice; i <= askMaxPrice; ++i) {
			lastVol = lastSnap.getAskVolOfPrice(i);
			nowVol = nowSnap.getAskVolOfPrice(i);
			if (nowVol < lastVol) {
				volAna.curAskVolDec += lastVol - nowVol;
				volAna.askDecAvgPrice += i * (lastVol - nowVol);
			}
		}
		if (volAna.curAskVolDec != 0) {
			volAna.askDecAvgPrice /= volAna.curAskVolDec;
			volAna.askDecAvgPriceDiff = volAna.askDecAvgPrice - lastSnap.ask1;
		}
	}

	volAna.curBidVolDec = 0;
	volAna.bidDecAvgPrice = 0;
	volAna.bidDecAvgPriceDiff = 0;
	int bidMaxPrice = lastSnap.bid1;
	if (bidMaxPrice == 0) {	//it was dnBan, so no bidVol traded
	}
	else {
		int bidMinPrice = _MAX_(lastSnap.minValidBid(), nowSnap.minValidBid());
		if (bidMinPrice == 0) {//keep dnBan,so no bidVol traded
			return;
		}

		for (int i = bidMinPrice; i <= bidMaxPrice; ++i) {
			lastVol = lastSnap.getBidVolOfPrice(i);
			nowVol = nowSnap.getBidVolOfPrice(i);
			if (nowVol < lastVol) {
				volAna.curBidVolDec += lastVol - nowVol;
				volAna.bidDecAvgPrice += i * (lastVol - nowVol);
			}
		}
		if (volAna.curBidVolDec != 0) {
			volAna.bidDecAvgPrice /= volAna.curBidVolDec;
			volAna.bidDecAvgPriceDiff = lastSnap.bid1 - volAna.bidDecAvgPrice;
		}
	}
}



}