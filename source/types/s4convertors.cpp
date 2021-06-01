#include "types/s4convertors.h"
#include <math.h>
namespace S4{

//
#include "double-conversion/diy-fp.cc"
#include "double-conversion/fixed-dtoa.cc"
#include "double-conversion/bignum.cc"
#include "double-conversion/bignum-dtoa.cc"
#include "double-conversion/cached-powers.cc"
#include "double-conversion/fast-dtoa.cc"
#include "double-conversion/strtod.cc"
#include "double-conversion/double-conversion.cc"

static double_conversion::DoubleToStringConverter g_dtoa_converter(
double_conversion::DoubleToStringConverter::NO_FLAGS,
"INF",
"NAN",
'e',
-DoubleConvertor::SIGNIFICANT_DIGITS,
DoubleConvertor::SIGNIFICANT_DIGITS,
DoubleConvertor::SIGNIFICANT_DIGITS - 1,
0);

static double_conversion::StringToDoubleConverter g_atod_converter(
	double_conversion::StringToDoubleConverter::NO_FLAGS,
	std::numeric_limits<double>::quiet_NaN(),
	std::numeric_limits<double>::quiet_NaN(),
	"INF",
	"NAN");
double DoubleConvertor::fast_strtod(const char * buffer, int size, int * processed_chars)
{
	return g_atod_converter.StringToDouble(buffer, size, processed_chars);
}

int DoubleConvertor::fast_dtoa(char * buffer, int size, double value)
{
	double_conversion::StringBuilder builder(buffer, size);
	if (!g_dtoa_converter.ToPrecision(value, DoubleConvertor::SIGNIFICANT_DIGITS, &builder))
	{
		builder.Reset();
		return 0;
	}

	builder.TrimTrailingZeros();
	return builder.position();
}

int DoubleConvertor::fast_fixed_dtoa(char * buffer, int size, double value)
{
	double_conversion::StringBuilder builder(buffer, size);
	if (!g_dtoa_converter.ToFixed(value, DoubleConvertor::SIGNIFICANT_DIGITS, &builder))
	{
		builder.Reset();
		return 0;
	}

	return builder.position();
}

}//namespace S4
