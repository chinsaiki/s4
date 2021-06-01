#pragma once
#include "s4info.h"
#include "types/s4type.h"
#include "common/s4logger.h"

namespace S4 {

class infK_t : public info_t
{
public:
	infK_t():
		info_t(),
		_tMode(tUNKNOWN)
	{}

	infK_t(const dayK_t& pK) :
		open(pK.open),
		high(pK.high),
		low(pK.low),
		close(pK.close),
		amount(pK.amount),
		volume(pK.volume),
		//preClose(pK.preClose),
		_liutong(0),
		_zong(0),
		_factor(1.0),
		_tMode(tDAY),
		_fh_baseTime(0),
		_turnOver(0)
	{
		_date = pK.date;
		_MinmuSec = 0;
		_time = date_to_utc( pK.date );
		if (pK.reserve >> 31) {
			volume *= 100;
			TRACE("{} volume ovf", pK.date);
		}
	};

	infK_t(const minuK_t& pK) :
		open(pK.open),
		high(pK.high),
		low(pK.low),
		close(pK.close),
		amount(pK.amount),
		volume(pK.volume),
		//preClose(pK.preClose),
		_liutong(0),
		_zong(0),
		_factor(1.0),
		_tMode(tMINU),
		_fh_baseTime(0),
		_turnOver(0)
	{
		_date = utc_to_date(pK.time, &_MinmuSec);
		_time = pK.time;
	};

	infK_t(const infK_t& pK) :
		open(pK.open),
		high(pK.high),
		low(pK.low),
		close(pK.close),
		amount(pK.amount),
		volume(pK.volume),
		//preClose(pK.preClose),
		_liutong(pK._liutong),
		_zong(pK._zong),
		_factor(pK._factor),
		_tMode(pK._tMode),
		_fh_baseTime(pK._fh_baseTime),
		_turnOver(pK._turnOver)
	{
		_date = pK._date;
		_MinmuSec = pK._MinmuSec;
		_time = pK._time;
	};

	price_t open;
	price_t high;
	price_t low;
	price_t close;
	amount_t amount;
	vol_share_t volume;
	//int preClose;
	fq_share_t _liutong;
	fq_share_t _zong;
	fq_index_t _factor;

	turnover_t _turnOver;
	/*int open_fq;
	int high_fq;
	int low_fq;
	int close_fq;*/
	const enum timeMode_t _tMode;
	time_t _fh_baseTime;

	infK_t& operator=(const infK_t& pK) {
		if(this->_tMode != pK._tMode)
			throw  InfoError("Cannot apply 'operator=' between <dayK> and <minuK>!");;
		this->open = pK.open;
		this->high = pK.high;
		this->low = pK.low;
		this->close = pK.close;
		this->amount = pK.amount;
		this->volume = pK.volume;
		//this->preClose = pK.preClose;
		this->_liutong = pK._liutong;
		this->_zong = pK._zong;
		this->_factor = pK._factor;
		this->_turnOver = pK._turnOver;
		this->_fh_baseTime = pK._fh_baseTime;
		this->_date = pK._date;
		this->_MinmuSec = pK._MinmuSec;
		this->_time = pK._time;
		return *this;
	}

	inline price_t prcAvg(int price_precise = 100) const { return static_cast<int>(amount * price_precise / volume); };
	inline price_t prcAvg_fq(int price_precise = 100) const { 
		return static_cast<int>(amount * price_precise * _factor / volume); 
	};
	inline price_t prcAvg_fq(float factor, int price_precise = 100) const { 
		return static_cast<int>(amount * price_precise * _factor / volume * _factor / factor);
	};

	inline price_t high_fq() const { return static_cast<price_t>(high * _factor); };
	inline price_t low_fq() const { return static_cast<price_t>(low * _factor); };
	inline price_t open_fq() const { return static_cast<price_t>(open * _factor); };
	inline price_t close_fq() const { return static_cast<price_t>(close * _factor); };

	inline price_t high_fq(fq_index_t factor) const {
		return static_cast<int>(high * _factor / factor);
	};
	inline price_t low_fq(fq_index_t factor) const {
		return static_cast<int>(low * _factor / factor);
	};
	inline price_t open_fq(fq_index_t factor) const {
		return static_cast<int>(open * _factor / factor);
	};
	inline price_t close_fq(fq_index_t factor) const {
		return static_cast<int>(close * _factor / factor);
	};
};

class infKQ_t : public infQ_t<infK_t>
{
public:
	void add_vec_dayK(vec_dayK_t& KQ);
	void add_vec_minuK(vec_minuK_t& mK);
	void calcFQ(const vec_gbbd_t* gbbd, const vec_fhps_t* fhps);

private:
	timeMode_t _tMode = tUNKNOWN;
private:
	void calcFH(const vec_fhps_t* fhps);
	void calcGB(const vec_gbbd_t* gbbd);

};

}
