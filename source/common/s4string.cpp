
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4244 )
#endif

#include "common/s4string.h"
#include "common/s4logger.h"
#include <iostream>
#include <fstream>

#include <algorithm>

#if defined(_MSC_VER)
#pragma warning( pop )
#endif


namespace S4{

void string_replace(const std::string& oldValue, const std::string& newValue,
	std::string& str)
{
	for (std::string::size_type pos = str.find(oldValue);
		pos != std::string::npos;
		pos = str.find(oldValue, pos))
	{
		str.replace(pos, oldValue.size(), newValue);
		pos += newValue.size();
	}
}

std::string string_replace(const std::string& oldValue, const std::string& newValue,
	const std::string& str)
{
    std::string rslt(str);
    string_replace(oldValue, newValue, rslt);
    return std::move(rslt);
}

std::string string_toUpper(const std::string& value)
{
	std::string copy(value);

	std::transform(copy.begin(), copy.end(), copy.begin(), toupper);

	return copy;
}

std::string string_toLower(const std::string& value)
{
	std::string copy(value);

	std::transform(copy.begin(), copy.end(), copy.begin(), tolower);

	return copy;
}

std::string string_strip(const std::string& value)
{
	if (!value.size())
		return value;

	size_t startPos = value.find_first_not_of(" \t\r\n");
	size_t endPos = value.find_last_not_of(" \t\r\n");

	if (startPos == std::string::npos)
		return value;

	return std::string(value, startPos, endPos - startPos + 1);
}


std::string string_strip_atTail(const std::string& value, const char * dropChars)
{
	if (!value.size())
		return value;

	size_t endPos = value.find_last_not_of(dropChars);

	if (endPos == std::string::npos)
		return value;

	return std::string(value, 0, endPos + 1);
}

std::vector<std::string> string_strip(const std::string& src, const std::string& splitor)
{
	std::vector<std::string> v;
	std::string::size_type pos1, pos2;
    pos2 = src.find(splitor);
    pos1 = 0;
    while(std::string::npos != pos2)
    {
        v.push_back(src.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + splitor.size();
        pos2 = src.find(splitor, pos1);
    }
    if(pos1 != src.length()){
        v.push_back(src.substr(pos1));
    }

	return std::move(v);
}



bool write_string_to_file(const std::string & file_name, const std::string & str, bool newFile) 
{ 
	try {
		std::ofstream	OsWrite(file_name, newFile? (std::ofstream::out):(std::ofstream::app));
		OsWrite << str;
		OsWrite << std::endl;
		OsWrite.close();
		return true;
	}
	catch (std::exception& e) {
		ERR("write string to file({}) fail:{}", file_name, e.what());
		throw e;
	}
}

}//namespace S4


