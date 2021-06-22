#pragma once

#include "common/s4mktCode.h"
#include "types/s4type_time.h"
#include <string>

#define _DOOMSDAY_ (21001231)

namespace S4
{

//Key-Time
const time_minuSec_t KTP_STK_OPENCALL = 91500;     // 'S', 'O'
const time_minuSec_t KTP_STK_PBREAK = 92500;       // 'p'
const time_minuSec_t KTP_STK_TRADE1 = 93000;       // 'T'
const time_minuSec_t KTP_STK_BREAK = 113000;       // 'B'
const time_minuSec_t KTP_STK_TRADE2 = 130000;      // 'T'
const time_minuSec_t KTP_STK_CLOSECALL = 145700;   // 'C'
const time_minuSec_t KTP_STK_ENDING_T = 150000;    // 'E'
const time_minuSec_t KTP_STK_AFTERTRAD_S = 150500; // 'A'
const time_minuSec_t KTP_STK_ENDING_A = 153000;    // 'E'

const int KTM_STK_OPENCALL = 10;
const int KTM_STK_PBREAK = 5;       // 'p'
const int KTM_STK_TRADE1 = 120;     // 'T'
const int KTM_STK_BREAK = 90;       // 'B'
const int KTM_STK_TRADE2 = 117;     // 'T'
const int KTM_STK_CLOSECALL = 3;    // 'C'
const int KTM_STK_ENDING_T = 5;     // 'E'
const int KTM_STK_AFTERTRAD_S = 25; // 'A'
const int KTM_STK_ENDING_A = 0;     // 'E'

uint64_t nowTime_ms(void);

/**** 精度秒 ***/
//HHMMSS
time_minuSec_t nowMinuSec(void);

//return YYYYmmDD; & if minuSec!=nullptr: *minuSec=HHMMSS;
time_date_t utc_to_date(time_utcSec_t t, time_minuSec_t *minuSec = nullptr);

//%Y_%m_%d__%H_%M_%S
std::string utc_to_str(time_utcSec_t utc);

//%Y_%m_%d
std::string date_to_str(time_date_t date);

//%H_%M_%S
std::string utc_to_strMinu(time_utcSec_t utc);

//date:YYYYMMDD  minuSec=HHMMSS
time_utcSec_t date_to_utc(time_date_t date, time_minuSec_t minuSec = 0);

//mdate="2010-11-30"  mtime="11:30:45"
void str_to_tm(char *mdate, char *mtime, struct tm *mtm);

//utcdiff=-8 for Asia/Shanghai; but we set to 0 during whole program
time_utcSec_t tm_to_utc(const struct tm *ltm, int utcdiff = 0);

//YYYYmmDD [19901219~
bool chk_stk_date_legal(time_date_t date);

/**** 精度m秒 ***/
std::string ms_to_str(uint64_t ms);

/**** 精度n秒 ***/

/**** L2 帧数据中的时间戳转换工具 ***/
std::string ssz_L2_timeString(uint64_t TransactTime);
std::string ssh_L2_timeString(uint64_t TransactTime);

/**** RAII型的时间报告工具 ***/
class rptTime
{
public:
    rptTime();
    explicit rptTime(const std::string &target);
    rptTime(const std::string &target, double rptGate);

    ~rptTime();

    void estimate(size_t i, size_t i_max, size_t gap = 10);

private:
    const std::string m_target;
    timeb tb;
    double _rptGate;
};

bool isDealTime_stock(time_minuSec_t minuSec, mktCodeI_t mktCode);

} //namespace S4