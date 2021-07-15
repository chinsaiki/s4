#pragma once

#include "httplib.h"
#include "parse_url.h"

namespace S4 {

inline
bool download(const std::string& url, std::string& download_data)
{
    // rptTime rt("imgDownload");
    HTTPURL http(url);
	std::string server_base(http.protocol + "://" + http.domain + ":" + std::to_string(http.port));
	std::string req_path(http.path + "?" + http.query);
	httplib::Client http_client(server_base.c_str());
	http_client.set_connection_timeout(5);
	http_client.set_read_timeout(30);
	httplib::Headers header = {
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
	httplib::Result res =
	//	http_client.Get(req_path.c_str(), header, [](uint64_t len, uint64_t total) {
	//	printf("\r%lld / %lld bytes => %d%% complete",
	//		len, total,
	//		(int)((len * 100 / total)));
	//	return true; // return 'false' if you want to cancel the request.
	//}
	//);
		http_client.Get(req_path.c_str(), header);

	if ((res == nullptr) || res->status != 200) {
		// LCL_ERR("downloaded {} failed!", url);
		if (res) {
			// LCL_ERR(" ret={} ", res->status);
		}
		else {
			// LCL_ERR(" res = NULL! ");
		}
		return false;
	}
	//std::cout << res->body << std::endl;
	// write_string_to_file(".\\basic_" + date + ".csv", res->body, true);
    download_data = std::move(res->body);
	// LCL_INFO("basic saved.");
    return true;
}

}