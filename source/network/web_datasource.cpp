#pragma once

#include "network/parse_url.h"
#include "network/web_datasource.h"
#include "common/s4logger.h"

CREATE_LOCAL_LOGGER("web_dts")

namespace S4 {

dts_web_t::dts_web_t(const std::string& server)
{
    HTTPURL http(server);
	_server_base = http.protocol + "://" + http.domain + ":" + std::to_string(http.port);
    _client = std::make_shared<httplib::Client>(_server_base.c_str());
	_client->set_connection_timeout(5);
	_client->set_read_timeout(30);

    _header = {
		  {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:89.0) Gecko/20100101 Firefox/89.0"},
		  {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"},
		  {"Accept-Language", "zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2"},
		  {"Accept-Encoding", "gzip, deflate"},
		  {"DNT", "1"},
		  {"Connection", "keep-alive"},
		  // {"Cookie", "userid=1626316540267_5q45zk3341; if_prov=cn0591; if_city=0591; prov=cn0591; city=0591; weather_city=fj_fz; region_ip=218.104.230.x; region_ver=1.2"},
		  {"Upgrade-Insecure-Requests", "1"},
		  {"Cache-Control", "max-age=0"},
	};
}

bool dts_web_t::get(const std::string& path, std::string& download_data)
{
	// httplib::Result res =
	//	http_client.Get(req_path.c_str(), header, [](uint64_t len, uint64_t total) {
	//	printf("\r%lld / %lld bytes => %d%% complete",
	//		len, total,
	//		(int)((len * 100 / total)));
	//	return true; // return 'false' if you want to cancel the request.
	//}
	//);

	httplib::Result res =_client->Get(path.c_str(), _header);
	if ((res == nullptr) || res->status != 200) {
        LCL_ERR("downloaded {}/{} failed!", _server_base, path);
		if (res) {
			LCL_ERR("ret={}", res->status);
		}
		else {
			LCL_ERR("res = NULL! ");
		}
		return false;
	}
    download_data = std::move(res->body);
    return true;
}

dts_sina_t::dts_sina_t():
    dts_web_t("http://vip.stock.finance.sina.com.cn")
{
}

bool dts_sina_t::get_today(int page, std::vector<sina_today_t>& data)
{
	const static std::vector<std::string> types = 
	{ 
		"/quotes_service/api/json_v2.php/Market_Center.getHQNodeData?num=80&sort=code&asc=0&node=hs_a&symbol=&_s_r_a=page&page=",
		"/quotes_service/api/json_v2.php/Market_Center.getHQNodeData?num=80&sort=code&asc=0&node=shfxjs&symbol=&_s_r_a=page&page=" 
	};
	int ok_nb = 0;
	std::vector<sina_today_t> ret;
	for (const auto& t : types) {
		std::string url = t + std::to_string(page);
		std::string download_data;
		if (get(url, download_data) && download_data.size()) {
			try {
				json j = json::parse(download_data);
				for (auto& json_data : j) {
					sina_today_t history_data;
					if (!sina_today_t::from_json(json_data, history_data)) {
						LCL_ERR("parse {} ret={} fail!", url, json_data.dump());
						continue;
					}
					ret.emplace_back(history_data);
				}
				ok_nb += (ret.size() != 0);
			}
			catch (std::exception& e) {
				LCL_ERR("parse {} fail:{}!", url, e.what());
			}
		}
	}
	std::swap(data, ret);
	return ok_nb > 0;
}


dts_ifeng_t::dts_ifeng_t():
	dts_web_t("http://api.finance.ifeng.com")
{
}

bool dts_ifeng_t::get_history(const std::string& code, ifeng_history_daily_t& data)
{
    data.record.clear();
    std::string url("/akdaily/?code=" + code + "&type=last");
    std::string download_data;
    if (!get(url, download_data)){
        return false;
    }
    try{
        json j = json::parse(download_data);
        return ifeng_history_daily_t::from_json(j, data);
    }
    catch (std::exception& e) {
        LCL_ERR("parse {} fail:{}!", url, e.what());
        return false;
    }
}



}
