#pragma once
#include <time.h>
#include <vector>
//#include "common/s4logger.h"
#include "common/s4exceptions.h"
#include "types/s4convertors.h"
#include "types/s4type.h"
#include "common/s4time.h"

#include <memory>

namespace S4 {
	class info_t
	{
	public:
		virtual std::string& toString() { 
			str = IntConvertor::convert(_date) + " " + IntConvertor::convert(_MinmuSec);
			return str; 
		}

		time_date_t _date;
		time_minuSec_t _MinmuSec;
		time_t _time;
		std::string str;
	};

	template<class T>
	class infQ_t : public std::vector<std::shared_ptr<T>>
	{
	public:
		infQ_t();
		~infQ_t();

		void newAtFront(void);
		void newAtBack(void);
		bool isNewAtBack(void) const;

		virtual void addQ(const infQ_t<T>& Q);
		virtual void addQ(const infQ_t<T>& Q, size_t lmt_size);

		//size_t getBase(void) { return _base; };
		void setBase(time_t now);
		void nextBase(time_t clk);
		bool done(void);
		bool isOTF(void) {
			return _otf;
		}

		const T* getInfo_ref(int i) const;
		const T* getInfo_abs(time_t clk) const;
		const T* getInfo_before(time_t dlt, time_t error) const;
		virtual const T* getLastInfo(time_t clk) const;

		const std::string& toString(int i) {
			T* mm = this->getInfo_ref(i);
			if (mm == NULL) {
				return _noMember;
			}
			return mm->toString();
		}

		void doOTF(void) {
			_otf = true;
		}
	protected:
		virtual void dropOld(size_t nb);
		void doSwap(void);

	private:
		bool _backIsNew;
		bool _has_base;
		size_t _base;
		time_t _now;

		bool _otf;

		const std::string _noMember = "no member";
	private:

	};

	template<class T>
	S4::infQ_t<T>::infQ_t() :
		std::vector< std::shared_ptr<T>>(),
		_backIsNew(true),
		_has_base(false),
		_base(0),
		_now(0),
		_otf(false)
	{}

	template<class T>
	S4::infQ_t<T>::~infQ_t()
	{
		//destroy();
	}

	template<class T>
	void S4::infQ_t<T>::newAtFront(void)
	{
		if (!_backIsNew) return;
		doSwap();
		_backIsNew = false;
	}

	template<class T>
	void S4::infQ_t<T>::newAtBack(void)
	{
		if (_backIsNew) return;
		doSwap();
		_backIsNew = true;
	}

	template<class T>
	bool S4::infQ_t<T>::isNewAtBack(void) const {
		return _backIsNew;
	}

	template<class T>
	void S4::infQ_t<T>::addQ(const S4::infQ_t<T>& Q)
	{
		//resize(size() + K.size());
		if (!_backIsNew)	//keep order to input
			doSwap();
		for (typename infQ_t<T>::const_iterator i = Q.begin(); i != Q.end(); ++i)
		{
			if(infQ_t<T>::size()==0 || (*i)->_time > (*infQ_t<T>::rbegin())->_time)	//keep back is new
				infQ_t<T>::push_back(*i);
		}
		if (!_backIsNew)	//keep order to before
			doSwap();
	}

	template<class T>
	void S4::infQ_t<T>::addQ(const infQ_t<T>& Q, size_t lmt_size)	//TODO: what about base?
	{
		addQ(Q);
		if (lmt_size * 2 < infQ_t<T>::size())//cut to half
			dropOld(infQ_t<T>::size() - lmt_size);
	}

	template<class T>
	void S4::infQ_t<T>::doSwap(void)	//TODO: what about base?
	{
		for (size_t i = 0; i < infQ_t<T>::size() / 2; ++i)
		{
			std::swap((*this)[i], (*this)[infQ_t<T>::size() - 1 - i]);
		}
	}

	template<class T>
	void S4::infQ_t<T>::dropOld(size_t nb)	//TODO: what about base?
	{
		if (nb >= infQ_t<T>::size()) {
			infQ_t<T>::clear();
			return;
		}

		if (!_backIsNew)	//keep order to input
			doSwap();

		std::vector<std::shared_ptr<T>> newQ(infQ_t<T>::begin() + nb, infQ_t<T>::end());
		infQ_t<T>::clear();
		
		for (auto& i : newQ)
			infQ_t<T>::emplace_back(i);

		if (!_backIsNew)	//keep order to before
			doSwap();
	}


	template<class T>
	void infQ_t<T>::setBase(time_t now)
	{
		if (infQ_t<T>::size() == 0) {
			_has_base = false;	//
			_base = 1;
			_now = 99999999 + 1;
			return;
		}
		if (!isNewAtBack())
			newAtBack();

		if (_otf) {
			_base = static_cast<int>(infQ_t<T>::size()) - 1;	//
			_now = infQ_t<T>::back()->_time;
			_has_base = utc_to_date(now) == utc_to_date(_now);	//same day at bgn
			return;
		}

		if (now > infQ_t<T>::back()->_time) {
			_base = infQ_t<T>::size()-1;
			_now = infQ_t<T>::back()->_time;
			_has_base = false;
			return;
		}
		if (now < infQ_t<T>::front()->_time) {
			_base = 0;
			_now = 0;
			_has_base = false;
			return;
		}

		size_t low = 0, high = infQ_t<T>::size(), mid;
		while (low <= high) {
			mid = low + (high - low) / 2;
			if (now == (*this)[mid]->_time) {
				_base = mid;
				_now = now;
				_has_base = true;
				return;
			}
			else if ((*this)[mid]->_time > now) {
				high = mid - 1;
			}
			else {
				low = mid + 1;
			}
		}

		_base = high;
		_now = (*this)[high]->_time;
		_has_base = false;
		return;

		//for (int i = static_cast<int>(size()) - 1; i >= 0; --i) {
		//	if ((*this)[i]->_time == now) {
		//		_base = i;
		//		_now = now;
		//		_has_base = true;
		//		return;
		//	}
		//	if ((*this)[i]->_time < now) {
		//		_base = i;
		//		_now = (*this)[i]->_time;
		//		_has_base = false;
		//		return;
		//	}
		//}
		//_base = 0;	// all big than now
		//_now = (*this)[0]->_time;
		//_has_base = false;	//
	}

	template<class T>
	bool infQ_t<T>::done(void)
	{
		return _base >= infQ_t<T>::size();
	}

	template<class T>
	void infQ_t<T>::nextBase(time_t clk)
	{
		if (!infQ_t<T>::size())
			return;

		if (_otf) {
			if (_now == infQ_t<T>::back()->_time) {//nothing new
				_has_base = false;
			}
			else {
				_base = static_cast<int>(infQ_t<T>::size()) - 1;
				_now = infQ_t<T>::back()->_time;
				_has_base = true;	//
			}
			return;
		}

		//if (done()) {
		//	_has_base = false;	//
		//	return;
		//}
		if (_now > clk) {
			_has_base = false;	//
			return;
		}
		else if (_now == clk) {
			_has_base = true;	//
			return;
		}

		_base++;
		if (_base >= infQ_t<T>::size()) {
			_base--;//防止一直无法使用最后一个
			_has_base = false;	//
			return;
		}

		if ((*this)[_base]->_time == clk) {
			_has_base = true;
			_now = (*this)[_base]->_time;
			return;
		}

		if ((*this)[_base]->_time < clk) {
			//WARN("[W] nextBase start at quite less than clk!");
			for (; _base < infQ_t<T>::size(); ++_base) {
				if ((*this)[_base]->_time == clk) {	//匹配
					_now = clk;
					_has_base = true;
					return;
				}
				if ((*this)[_base]->_time > clk) {	//越界
					_base--;
					break;
				}
			}
			if (_base >= infQ_t<T>::size()) {	//未找到，使用最后一个
				_base--;
			}

			if ((*this)[_base]->_time > _now) {	//越界的前一个或最后一个是未处理过的bar
				_has_base = true;	//使用此bar
			}
			else {
				_has_base = false;	//
			}
			_now = (*this)[_base]->_time;
			return;
		}

		//now<clk，且下一个bar>clk，回退，继续等
		_has_base = false;
		_base--;
		_now = (*this)[_base]->_time;
		return;

	}

	template<class T>
	const T* infQ_t<T>::getInfo_ref(int i) const
	{
		int j(i);
		if (i > 0) {
			//throw InfoError("Cannot get info " + IntConvertor::convert(i) + " entries after " + UTC_to_str_s(&_now).str);
			j = -j;
		}

		if (!_has_base) {
			return NULL;
		}

		if (_base + j >= infQ_t<T>::size()) {	//equals: -i > _base
			return NULL;
		}
		return (*this)[_base + j].get();
	}

	template<class T>
	const T* infQ_t<T>::getInfo_before(time_t dlt, time_t error) const
	{
		if (!_has_base || _base==0) {
			return NULL;
		}

		T* pBase = (*this)[_base].get();
		
		if(pBase->_time < dlt)
			return NULL;

		time_t target = pBase->_time - dlt;

		int i=_base-1;
		for(; i>0; --i){
			if((*this)[i]->_time <= target)
				break;
		}

		if(target > (*this)[i]->_time && target - (*this)[i]->_time < error)
			return (*this)[i].get();

		if(target < (*this)[i+1]->_time && (*this)[i]->_time - target < error)
			return (*this)[i+1].get();

		return NULL;
	}

	template<class T>
	const T* infQ_t<T>::getInfo_abs(time_t clk) const
	{
		if (infQ_t<T>::size() == 0) {
			return NULL;
		}
		//if (!isNewAtBack())
		//	newAtBack();
		assert(isNewAtBack());

		if ((*this)[0]->_time > clk)
			return NULL;
		if (infQ_t<T>::back()->_time < clk)
			return NULL;

		int low = 0, high = static_cast<int>(infQ_t<T>::size()), mid;
		while (low <= high) {
			mid = low + (high - low) / 2;
			if (clk == (*this)[mid]->_time) {
				return (*this)[mid].get();
			}
			else if (clk < (*this)[mid]->_time) {
				high = mid - 1;
			}
			else {
				low = mid + 1;
			}
		}
		return NULL;
	}

	

	template<class T>
	const T* infQ_t<T>::getLastInfo(time_t clk) const
	{
		if (infQ_t<T>::size() == 0) {
			return NULL;
		}

		//if (!isNewAtBack())
		//	newAtBack();
		assert(isNewAtBack());

		if (infQ_t<T>::back()->_time < clk) {
			return infQ_t<T>::back().get();
		}

		if ((*this)[0]->_time > clk)
			return NULL;

		size_t low = 0, high = infQ_t<T>::size(), mid;
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
		return (*this)[high].get();
	}


	//template<class T>
	//T* infQ_t<T>::getLastInfo(time_t clk)
	//{
	//	if (size() == 0) {
	//		return NULL;
	//	}
	//	if (!isNewAtBack())
	//		newAtBack();

	//	for (int i = size() - 1; i >= 0; --i) {
	//		if ((*this)[i]->_time == clk) {
	//			return (*this)[i];
	//		}
	//		else if ((*this)[i]->_time < clk) {
	//			return (*this)[i];
	//		}
	//	}
	//	return NULL;

	//}




}
