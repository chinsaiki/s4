#include "tdx/tdx_local.h"
#ifdef _WIN32
#  include <io.h>
#endif
#include <fstream>
#include "common/s4logger.h"
#include "common/s4time.h"
#include <assert.h>

using namespace std;

CREATE_LOCAL_LOGGER("tdx_lcl")

namespace S4{
namespace TDX{

tdxLocal_t::tdxLocal_t(const std::string& tdxPath)
{
	//size_t pos=0;
	//size_t end;
	//string path;
	//
	//do {
	//	end = tdxPath.find(';', pos);
	//	path = tdxPath.substr(pos, end - pos);
	//	if (_access(path.c_str(), 4) != 0) {
	//		//perror((string("tdxLocal_t tdxPath ") + path).c_str());
	//	}
	//	else {
	//		LCL_INFO("find valid TDX={}", path);
	//		break;
	//	}
	//	pos = end+1;
	//} while (pos <= tdxPath.size());
	m_tdxPath = tdxPath;
}

void tdxLocal_t::toMinuK(const struct tdxRawMinuK_t& rawData, struct minuK_t& minuK)
{
	parseTime(rawData, minuK);
	minuK.open = fPrice_to_iPrice(rawData.open);
	minuK.close = fPrice_to_iPrice(rawData.close);
	minuK.high = fPrice_to_iPrice(rawData.high);
	minuK.low = fPrice_to_iPrice(rawData.low);
	minuK.volume = rawData.volume;
	minuK.amount = rawData.amount;
}

int tdxLocal_t::parseTime(const struct tdxRawMinuK_t& rawData, struct minuK_t& minuK) {
	struct tm tmTime;
	tmTime.tm_year = (rawData.date >> 11) + 2004 - 1900;
	tmTime.tm_mon = (rawData.date & 0x7ff) / 100;
	tmTime.tm_mday = (rawData.date & 0x7ff) % 100;
	tmTime.tm_mon--;	//[0,11]

	tmTime.tm_hour = rawData.time / 60;
	tmTime.tm_min = rawData.time % 60;
	tmTime.tm_isdst = 0;
	tmTime.tm_sec = 0;

	minuK.time = tm_to_utc(&tmTime);
	// time_t t = mktime(&minuK.time);	//take TimeZone as Asia/Shanghai
	// tm tmTime = *gmtime(&t);		//take no TimeZone, so time shifts if t was made by mktime.
	// tm tmTimeb = *gmtime(&minuK.utcSec);
	return 0;
}

// [bgnDate, endDate] [20140105, 20181231]
int tdxLocal_t::readMinuK_raw(const std::string& code, struct tdxRawMinuK_t *& rawData, const int minuFreq)
{
	return 0;
}

bool tdxLocal_t::readMinuK(const std::string& code, std::vector<struct minuK_t>& Kque,
	const int bgnDate, const int endDate,
	const int minuFreq)
{
	return true;
}


bool tdxLocal_t::readMinuK_nb(const std::string& code, const int endDate,
	const int nb_preEnd, std::vector<struct minuK_t>& Kque,
	const int minuFreq)
{
	return true;
}


// [bgnDate, endDate]
int tdxLocal_t::readDayK_raw(mktCodeI_t mktCode, struct dayK_t *& rawData)
{
	char path[1024];
	int bReadFail;
	int size;
	int dataCnt = 0;

	std::string code = mktCodeInt_to_mktCodeStr(mktCode);
	if (isSHmkt(mktCode)){
		sprintf(path, "%s%s%s.day", m_tdxPath.c_str(), m_dayFolderSH.c_str(), code.c_str());
	}else{
		sprintf(path, "%s%s%s.day", m_tdxPath.c_str(), m_dayFolderSZ.c_str(), code.c_str());
	}
	
	FILE *f = fopen(path, "rb");
	if (f == NULL) {
		LCL_ERR("tdxLocal_t::readDayK path={} unreadable!", path);
		return -1;
	}

	bReadFail = fseek(f, 0, SEEK_END);//
	if (bReadFail < 0){
		LCL_ERR("fseek to END fail! path={}", path);
	}
	size = ftell(f);
	dataCnt = size / sizeof(struct dayK_t);

	if(dataCnt == 0){
		fclose(f);
		return -1;
	}

	rawData = new struct dayK_t[dataCnt];
	bReadFail = fseek(f, 0, SEEK_SET);
	if (bReadFail < 0){
            LCL_ERR("fseek SET0 fail! path={}", path);
        }

	fread(rawData, dataCnt, sizeof(struct dayK_t), f);   //¶Áwhole

	fclose(f);

	return dataCnt;
}

bool tdxLocal_t::readDayK(const std::string& mktCodeStr, vec_dayK_t& Kque,
	int bgnDate, int endDate)
{
	mktCodeI_t mktCode = mktCodeStr_to_mktCodeInt(mktCodeStr);
	return readDayK(mktCode, Kque, bgnDate, endDate);
}

bool tdxLocal_t::readDayK(mktCodeI_t mktCode, vec_dayK_t& Kque,
	int bgnDate, int endDate)
{
	assert(endDate >= bgnDate);

	Kque.clear();

	struct dayK_t * rawData;
	int dataCnt = readDayK_raw(mktCode, rawData);
	if (dataCnt < 0) return false;

	int bgn;
	int i;
	const int search_gap = 16;
	if (rawData[0].date >= bgnDate) {
		bgn = i = 0;
	}else if(rawData[dataCnt - 1].date < bgnDate){
		return true;
	}else{
		//jump to first date
		for(i=0; i < dataCnt; i+=search_gap){
			if (i >= dataCnt || rawData[i].date >= bgnDate)
				break;
		}
		if (i > 0) {
			for (int j=0; j < search_gap && i > 0; j++, i--) {
				if (i < dataCnt && rawData[i].date <= bgnDate)
					break;
			}
			if (rawData[i].date < bgnDate)	//i = first day in scope
				i++;
		}
		bgn = i;
		assert(rawData[i].date >= bgnDate && (i == 0 || rawData[i-1].date < bgnDate));
	}
	
	if (rawData[dataCnt-1].date <= endDate){
		i = dataCnt;
	}
	else if (rawData[0].date > endDate) {
		return true;
	}
	else{
		//jump to last date
		for (; i < dataCnt; i+=search_gap) {
			// if (rawData[i].date < bgnDate)
			// 	continue;
			if (rawData[i].date >= endDate)
				break;
			//Kque.push_back(rawData[i]);
		}
		for(; i > 0; i--){
			if (rawData[i].date <= endDate)
				break;
		}
		i++;//i = first day out scope
		assert(rawData[i].date > endDate && (i == 0 || rawData[i - 1].date <= endDate));
	}

	size_t len = i - bgn;
	assert(len<= (size_t)dataCnt);

	Kque.resize(len);
	memcpy(&Kque[0], &rawData[bgn], sizeof(dayK_t)*len);


	//printf("readDayK(%s) done, [%s : %s] nb=%u\n", code.c_str(), mmQ.cmin().getString(), mmQ.cmax().getString(), Kque.size());

	delete[] rawData;
	return true;
}

bool tdxLocal_t::readDayK_nb(const std::string& mktCodeStr, int endDate,
		int nb_preEnd, vec_dayK_t& Kque)
{
	mktCodeI_t mktCode = mktCodeStr_to_mktCodeInt(mktCodeStr);
	return readDayK_nb(mktCode, endDate, nb_preEnd, Kque);
}

bool tdxLocal_t::readDayK_nb(mktCodeI_t mktCode, int endDate, 
		int nb_preEnd, vec_dayK_t& Kque)
{
	Kque.clear();

	struct dayK_t * rawData;
	int dataCnt = readDayK_raw(mktCode, rawData);
	if (dataCnt < 0) return false;

	int endI = 0, i;
	if (rawData[dataCnt-1].date <= endDate){
		i = dataCnt;
	}else{
		const int search_gap = 16;
		for (i = 0; i < dataCnt; i += search_gap) {
			if (rawData[i].date > endDate)
				break;
		}
		for(; i > 0; i--){
			if (rawData[i].date <= endDate)
				break;
		}
		i++;//i = first day out scope
	}

	endI = i;
	i -= nb_preEnd;
	if (i < 0)i = 0;

	size_t len = endI - i;
	assert(len<= (size_t)dataCnt);

	Kque.resize(len);
	memcpy(&Kque[0], &rawData[i], sizeof(dayK_t)*len);

	//printf("readDayK(%s) done, [%s : %s] nb=%u\n", code.c_str(), mmQ.cmin().getString(), mmQ.cmax().getString(), Kque.size());

	delete[] rawData;
	return true;
}

}
}
