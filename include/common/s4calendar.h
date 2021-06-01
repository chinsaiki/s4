#pragma once
#include "common/s4logger.h"
#include "common/s4time.h"
#include <string>
#include <vector>
#include <time.h>

namespace S4{

class calendar_t
{
public:
	calendar_t(time_t bgn, time_t end) :
		m_bgn_time(bgn), m_end_time(end) {};

	//0=oldest, -1=newest
	int getDate(int idx) const
	{
		if (idx >= 0) {
			if ((size_t)idx >= size()) {
				ERR("calendar getDate({:d}) ovf!\n", idx);
				return 20840328;
			}
			return m_trading_days[idx];
		}
		else
		{
			if ((size_t)(-idx) > size()) {
				ERR("calendar getDate({:d}) - ovf!\n", idx);
				return 0;
			}
			return m_trading_days[size() + idx];
		}
	};

	int newest() const { return m_trading_days.back(); };
	int oldest() const { return m_trading_days.front(); };

	size_t size() const { return m_trading_days.size(); };
	//date:YYYYMMDD; ret=UTC
	time_t getBgnTime(int date) const
	{
		return date_to_utc(date) + m_bgn_time;
	};
	//date:YYYYMMDD; ret=UTC
	time_t getEndTime(int date) const
	{
		return date_to_utc(date) + m_end_time;
	};

	void push_back(int date) {
		m_trading_days.push_back(date);
	};

	const std::vector<int>& getDaysAll(void) const {
		return m_trading_days;
	}
	//void setBgnTime(time_t t) { m_bgn_time = t; };
	//void setEndTime(time_t t) { m_end_time = t; };

	std::vector<int> getDaysRange(int bgn, int end) const {
		std::vector<int> dates;
		for (size_t i = 0; i != m_trading_days.size(); ++i) {
			if (m_trading_days[i] >= bgn && m_trading_days[i] <= end) {
				dates.push_back(m_trading_days[i]);
			}
			if(m_trading_days[i] >= end)
				break;
		}
		return std::move(dates);
	}

private:
	//YYYYMMDD [0]=now        [size()-1]=oldest
	std::vector<int> m_trading_days;
	//add to UTC(YYYYMMDD)
	const time_t m_bgn_time;
	const time_t m_end_time;
};

class tdxCalendar_t : public calendar_t
{
public:
	tdxCalendar_t(const std::string& tdxRootPath);

private:

};

} //namespace S4