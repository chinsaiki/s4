#include "common/s4time.h"
#include "common/s4logger.h"

namespace S4{

uint64_t nowTime_ms(void)
{
	timeb tb;
	ftime(&tb);
	uint64_t t0 = tb.time * 1000 + tb.millitm;
	return t0;
}

time_minuSec_t nowMinuSec(void) {
	time_t sysClk;
	time(&sysClk);
	sysClk += 3600 * 8; //fix asia:shanghai

	int minu;
	utc_to_date(sysClk, &minu);

	return minu;
}

time_date_t utc_to_date(time_utcSec_t utc, time_minuSec_t* minuSec)
{
	struct tm m_tmTime = *gmtime(&utc);  //fix asia:shanghai inside
	time_date_t dateTime = (m_tmTime.tm_year + 1900) * 100 * 100 +
                            (m_tmTime.tm_mon + 1) * 100 +
                            (m_tmTime.tm_mday);

	if (minuSec != NULL)
		*minuSec = m_tmTime.tm_hour * 10000 + m_tmTime.tm_min * 100 + m_tmTime.tm_sec;

	return dateTime;
}

std::string utc_to_str(time_utcSec_t utc)
{
	char s[64];
	struct tm ltm = *gmtime(&utc);  //fix asia:shanghai inside
	strftime(s, 64, "%Y_%m_%d__%H_%M_%S", &ltm);
	return std::move(s);
}

std::string date_to_str(time_date_t date)
{
	char s[64];
	int year = date / 10000;
	int month = date % 10000;
	int day = month % 100;
	month = month / 100;
#ifdef _MSC_VER
	sprintf_s(s, sizeof(s), "%d_%02d_%02d", year, month, day);
#else
	snprintf(s, sizeof(s), "%d_%02d_%02d", year, month, day);
#endif
	return std::move(s);
}

//date:YYYYMMDD  minuSec=HHMMSS
time_utcSec_t date_to_utc(time_date_t date, time_minuSec_t minuSec)
{
	if (date < 0 || minuSec <0) {
		ERR("date_to_utc({:d}, {}) input illegal!\n", date, minuSec);
		return 0;
	}
	tm tmTime;
	int t = minuSec / 100;

	tmTime.tm_sec = minuSec % 100;
	tmTime.tm_min = t % 100;
	t /= 100;
	tmTime.tm_hour = t;
	if (tmTime.tm_sec > 60 || tmTime.tm_min >= 60 || tmTime.tm_hour >= 24) {
		ERR("date_to_utc({:d}, {}) input minuSec illegal!\n", date, minuSec);
	}

	t = date / 100;
	tmTime.tm_mday = date % 100;
	tmTime.tm_mon = t % 100 - 1;
	t = t / 100;
	tmTime.tm_year = t - 1900;

	return tm_to_utc(&tmTime);
}

void str_to_tm(char *mdate, char *mtime, struct tm* mtm)
{
	char *pstr;
	long year, month, day, hour, min, sec;

	year = strtol(mdate, &pstr, 10);
	month = strtol(++pstr, &pstr, 10);
	day = strtol(++pstr, &pstr, 10);

	hour = strtol(mtime, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
	min = strtol(pstr, &pstr, 10); while (!isdigit(*pstr)) ++pstr;
	sec = strtol(pstr, &pstr, 10);

	mtm->tm_sec = sec;
	mtm->tm_min = min;
	mtm->tm_hour = hour;
	mtm->tm_mday = day;
	mtm->tm_mon = month - 1;
	mtm->tm_year = year - 1900;
}


time_utcSec_t tm_to_utc(const struct tm *ltm, int utcdiff)
{
	const int mon_days[] =
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	const int mon_days_leap[] =
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	time_t tdays;
	int tyears, leaps, leap_this_year, utc_hrs;
	int i;

	tyears = ltm->tm_year - 70; // tm->tm_year is from 1900.
	leaps = (tyears + 2) / 4; // no of next two lines until year 2100.
	//i = (ltm->tm_year ¨C 100) / 100;
	//leaps -= ( (i/4)*3 + i%4 );
	leap_this_year = (tyears + 2) % 4 == 0;

	tdays = 0;
	if (leap_this_year) {
		leaps--;
		for (i = 0; i < ltm->tm_mon; i++) tdays += mon_days_leap[i];
	}
	else {
		for (i = 0; i < ltm->tm_mon; i++) tdays += mon_days[i];
	}

	tdays += ltm->tm_mday - 1; // days of month passed.
	tdays = tdays + (tyears * 365) + leaps;

	utc_hrs = ltm->tm_hour + utcdiff; // for your time zone.
	return (tdays * 86400) + (utc_hrs * 3600) + (ltm->tm_min * 60) + ltm->tm_sec;
}

bool chk_stk_date_legal(time_date_t date)
{
	const int mon_days_leap[] =
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	int year = date / 10000;
	int month = date % 10000;
	int day = month % 100;
	month = month / 100;

	if (year >= 1990 && year < 2030) {
		if (month >= 1 && month <= 12) {
			if (day >= 1 && day <= mon_days_leap[month - 1]) {
				return true;
			}
		}
	}
	return false;
}





rptTime::rptTime() :
	m_target(""),
    _rptGate(-1)
{
	ftime(&tb);
}

rptTime::rptTime(const std::string& target) :
	m_target(target),
    _rptGate(-1)
{
	ftime(&tb);
}

rptTime::rptTime(const std::string& target, double rptGate) :
	m_target(target),
    _rptGate(rptGate)
{
	ftime(&tb);
}
rptTime::~rptTime() {
	timeb te;
	ftime(&te);
	uint64_t t0 = tb.time * 1000 + tb.millitm;
	uint64_t t1 = te.time * 1000 + te.millitm;
	double dlt = (double)(t1 - t0) / 1000.0;
    if(dlt>=_rptGate)
	    INFO("{} used {:.3f} seconds.", m_target, dlt);
}

void rptTime::estimate(size_t now, size_t size, size_t gap)
{
	if (now==0 || now%gap != 0)
		return;
	timeb te;
	ftime(&te);
	uint64_t t0 = tb.time * 1000 + tb.millitm;
	uint64_t t1 = te.time * 1000 + te.millitm;
	double dlt = (double)(t1 - t0) / 1000.0;
	double tall = dlt * size / now;
	double tres = tall - dlt;
	INFO("{} progress:  [ {:d}/{:d} ]  used={:0.2f}s  ET(all={:0.2f}s; res={:0.2f}s)", m_target, now, size, dlt, tall, tres);
}

bool isDealTime_stock(time_minuSec_t minuSec, mktCodeI_t mktCode)
{
	//TODO: snapshot's time is not so precise.
	if (minuSec >= 91500 && minuSec <= 92500) {
		return true;
	}
	if (minuSec >= 93000 && minuSec <= 113000) {
		return true;
	}
	if (minuSec >= 130000 && minuSec <= 150000) {
		return true;
	}
	return false;
}

}//namespace S4
