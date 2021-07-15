#include "network/downloader.h"
#include "common/s4logger.h"
#include "common/s4conf.h"

#include "jsonTypes/his_ifeng_raw_t.h"

CREATE_LOCAL_LOGGER("test_http")

using namespace S4;

int main(int argc, char** argv)
{
    
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());
	std::string data;
    std::string url;

    LCL_INFO("test ifeng");
	url = "http://api.finance.ifeng.com/akdaily/?code=sz000875&type=last";
	if (!download(url, data)) {
		LCL_ERR("download fail");
		return -1;
	}
	LCL_INFO("{}", data);


    LCL_INFO("test sina");
    //json_v2.php
    //shfxjs | hs_a
	url = "http://vip.stock.finance.sina.com.cn/quotes_service/api/json_v2.php/Market_Center.getHQNodeData?num=80&sort=code&asc=0&node=shfxjs&symbol=&_s_r_a=page&page=1";
	if (!download(url, data)) {
		LCL_ERR("download fail");
		return -1;
	}
	LCL_INFO("{}", data);

	return 0;
}