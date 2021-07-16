#pragma once

#include "httplib.h"
#include "jsonTypes/sina_today_t.h"
#include "jsonTypes/ifeng_history_daily_t.h"

namespace S4 {

class dts_web_t
{
public:
	explicit dts_web_t(const std::string& server_base);
    virtual ~dts_web_t(){}

	bool get(const std::string& path, std::string& download_data);
protected:
	std::shared_ptr<httplib::Client> _client;
    httplib::Headers _header;
    std::string _server_base;
};

class dts_sina_t : public dts_web_t
{
public:
    dts_sina_t();
    virtual ~dts_sina_t(){}

    //data will be clean.
	bool get_today(int page, std::vector<sina_today_t>& data);
};

class dts_ifeng_t : public dts_web_t
{
public:
    dts_ifeng_t();
    virtual ~dts_ifeng_t(){}

    //code should be like: "sz000875"
    //data will be clean.
	bool get_history(const std::string& code, ifeng_history_daily_t& data);
};


}
