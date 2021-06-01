#pragma once

#include <string>
#include <string.h>
#include <assert.h>
#ifndef ASSERT
#define ASSERT(condition)         \
    assert(condition);
#endif

#include "common/s4exceptions.h"

namespace S4{

typedef int64_t signed_int;
typedef uint64_t unsigned_int;

#define UNSIGNED_VALUE_OF( x ) ( ( (x) < 0 ) ? unsigned_int(-x) : unsigned_int(x) )

//#define IS_SPACE( x ) ( x == ' ' )
#define IS_DIGIT( x ) ( unsigned_int( x - '0' ) < 10 )


static const char digit_pairs[201] = {
  "00010203040506070809"
  "10111213141516171819"
  "20212223242526272829"
  "30313233343536373839"
  "40414243444546474849"
  "50515253545556575859"
  "60616263646566676869"
  "70717273747576777879"
  "80818283848586878889"
  "90919293949596979899"
};

inline char* integer_to_string(char* buf, const size_t len, signed_int t)
{
	const bool isNegative = t < 0;
	char* p = buf + len;

	unsigned_int number = UNSIGNED_VALUE_OF(t);

	while (number > 99)
	{
		unsigned_int pos = number % 100;
		number /= 100;

		*--p = digit_pairs[2 * pos + 1];
		*--p = digit_pairs[2 * pos];
	}

	if (number > 9)
	{
		*--p = digit_pairs[2 * number + 1];
		*--p = digit_pairs[2 * number];
	}
	else
	{
		*--p = '0' + char(number);
	}

	if (isNegative)
		*--p = '-';

	return p;
}


/// Converts integer to/from a string
struct IntConvertor
{
	static std::string convert(signed_int value)
	{
		// buffer is big enough for significant digits and extra digit,
		// minus and null
		char buffer[std::numeric_limits<signed_int>::digits10 + 2];
		const char* const start
			= integer_to_string(buffer, sizeof(buffer), value);
		return std::string(start, buffer + sizeof(buffer) - start);
	}

	static bool convert(
		std::string::const_iterator str,
		std::string::const_iterator end,
		signed_int& result)
	{
		bool isNegative = false;
		signed_int x = 0;

		if (str == end)
			return false;

		if (*str == '-')
		{
			isNegative = true;
			if (++str == end)
				return false;
		}

		do
		{
			const unsigned_int c = *str - '0';
			if (c > 9) return false;
			x = 10 * x + c;
		} while (++str != end);

		if (isNegative) {
			x = unsigned_int(x);
			x = -x;
		}

		result = x;
		return true;
	}

	static bool convert(const std::string& value, signed_int& result)
	{
		return convert(move(value.begin()), move(value.end()), result);
	}

	static signed_int convert(const std::string& value)
	{
		signed_int result = 0;
		if (!convert(move(value.begin()), move(value.end()), result))
			throw FieldConvertError(value);
		else
			return result;
	}

};


/// Converts double to/from a string
struct DoubleConvertor
{

private:

	static double fast_strtod(const char * buffer, int size, int * processed_chars);

	static int fast_dtoa(char * buffer, int size, double value);

	static int fast_fixed_dtoa(char * buffer, int size, double value);

public:

	static const int SIGNIFICANT_DIGITS = 15;
	static const int BUFFFER_SIZE = 32;

	static std::string convert(double value, int padding = 0)
	{
		char result[BUFFFER_SIZE];
		char *end = 0;

		int size;
		if (value == 0 || value > 0.0001 || value < -0.0001)
		{
			size = fast_dtoa(result, BUFFFER_SIZE, value);
			if (size == 0)
				return std::string();

			if (padding > 0)
			{
				char* point = result;
				end = result + size - 1;
				while (*point != '.' && *point != 0)
					point++;

				if (*point == 0)
				{
					end = point;
					*point = '.';
					++size;
				}
				int needed = padding - (int)(end - point);

				if (needed > 0)
				{
					memset(++end, '0', needed);
					size += needed;
				}
			}
		}
		else
		{
			size = fast_fixed_dtoa(result, BUFFFER_SIZE, value);
			if (size == 0)
				return std::string();

			// strip trailing 0's
			end = result + size - 1;

			if (padding > 0)
			{
				int discard = SIGNIFICANT_DIGITS - padding;

				while ((*end == '0') && (discard-- > 0))
				{
					--end;
					--size;
				}
			}
			else
			{
				while (*end == '0')
				{
					--end;
					--size;
				}
			}
		}

		return std::string(result, size);
	}

	static bool convert(const std::string& value, double& result)
	{
		const char * i = value.c_str();

		// Catch null strings
		if (!*i) return false;
		// Eat leading '-' and recheck for null string
		if (*i == '-' && !*++i) return false;

		bool haveDigit = false;

		if (IS_DIGIT(*i))
		{
			haveDigit = true;
			while (IS_DIGIT(*++i));
		}

		if (*i == '.' && IS_DIGIT(*++i))
		{
			haveDigit = true;
			while (IS_DIGIT(*++i));
		}

		if (*i || !haveDigit) return false;

		int processed_chars;
		const int total_length = static_cast<int>(value.length());
		const double val = fast_strtod(value.c_str(), total_length, &processed_chars);

		if (processed_chars != total_length ||
			val != val /*test for quite NaN*/)
		{
			return false;
		}

		result = val;
		return true;
	}

	static double convert(const std::string& value)
	{
		double result = 0.0;
		if (!convert(value, result))
			throw FieldConvertError(value);
		else
			return result;
	}
};


/// Converts boolean to/from a string
struct BoolConvertor
{
	static std::string convert(bool value)
	{
		const char ch = value ? 'Y' : 'N';
		return std::string(1, ch);
	}

	static bool convert(const std::string& value, bool& result)
	{
		if (value.size() != 1) return false;
		switch (value[0])
		{
		case 'Y': result = true; break;
		case 'N': result = false; break;
		default: return false;
		}

		return true;
	}

	static bool convert(const std::string& value)
		//throw(FieldConvertError)
	{
		bool result = false;
		if (!convert(value, result))
			throw FieldConvertError(value);
		else
			return result;
	}
};

}//namespace S4
