#pragma once

#include <stdint.h>
#include <time.h>
#include <vector>
#include <string>
#include "types/s4type.h"

namespace S4{
namespace TDX{

class tdxLocal_t {
public:
	tdxLocal_t(const std::string& tdxRootPath);

	//clear Kque inside
	//[0]=oldest; [-1]=newest
	bool readDayK(const std::string& mktCode, vec_dayK_t& Kque,
		int bgnDate = 0, int endDate = 21001231);
	bool readDayK(mktCodeI_t mktCode, vec_dayK_t& Kque,
		int bgnDate = 0, int endDate = 21001231);

	bool readDayK_nb(const std::string& mktCode, int endDate, 
		int nb_preEnd, vec_dayK_t& Kque);
	bool readDayK_nb(mktCodeI_t mktCode, int endDate,
		int nb_preEnd, vec_dayK_t& Kque);

	//support 1minu / 5minu, clear Kque inside
	//[0]=oldest; [-1]=newest
	bool readMinuK(const std::string& code, std::vector<struct minuK_t>& Kque,
					const int bgnDate = 0, const int endDate = 21001231,
					const int minuFreq = 1);
	bool readMinuK_nb(const std::string& code, const int endDate,
		const int nb_preEnd, std::vector<struct minuK_t>& Kque,  
		const int minuFreq = 1);

	void readGBBQ(map_gbbd_t& gbbq, map_fhps_t& fhps) {
		//TODO
		gbbq.clear();
		fhps.clear();
	};
private:
	struct tdxRawMinuK_t
	{
		uint16_t date;
		uint16_t time;
		float open;
		float high;
		float low;
		float close;
		float amount;
		uint32_t volume;
		uint32_t rev;
	};

	std::string m_tdxPath;

	const std::string m_minuFolderSZ = "/vipdoc/sz/minline/";
	const std::string m_minuFolderSH = "/vipdoc/sh/minline/";

	const std::string m_dayFolderSZ = "/vipdoc/sz/lday/";
	const std::string m_dayFolderSH = "/vipdoc/sh/lday/";

private:
	static int parseTime(const struct tdxRawMinuK_t& rawData, struct minuK_t& minuK);
	static void toMinuK(const struct tdxRawMinuK_t& rawData, struct minuK_t& minuK);

	int readMinuK_raw(const std::string& code, struct tdxRawMinuK_t *& rawData, const int minuFreq);
	int readDayK_raw(mktCodeI_t mktCode, struct dayK_t *& rawData);
};


} // namespace TDX
} // namespace S4

