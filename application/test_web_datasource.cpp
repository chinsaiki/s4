#include "network/web_datasource.h"
#include "common/s4logger.h"
#include "common/s4conf.h"


CREATE_LOCAL_LOGGER("test_http")

using namespace S4;

int main(int argc, char** argv)
{
    
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

    dts_ifeng_t dts_ifeng;
    ifeng_history_daily_t ifeng_history_daily;
    if (!dts_ifeng.get_history("sz000997", ifeng_history_daily)){
        LCL_ERR("dts_ifeng.get_history fail!");
    }else{
        LCL_INFO("dts_ifeng.get_history done size={}", ifeng_history_daily.record.size());
        if (ifeng_history_daily.record.size() > 10){
            ifeng_history_daily.record.resize(10);
        }
        json j;
        ifeng_history_daily_t::to_json(j, ifeng_history_daily);
        LCL_INFO("first 10 datas = \n{}", j.dump(4));
    }

    dts_sina_t dts_sina;
    std::vector<sina_today_t> sina_today;
    if (!dts_sina.get_today(0, sina_today)){
        LCL_ERR("dts_sina.get_today fail!");
    }else{
        LCL_INFO("dts_sina.get_today done size={}", sina_today.size());
        if (sina_today.size() > 10){
            sina_today.resize(10);
        }
        LCL_INFO("first 10 datas =");
        for (auto& t : sina_today){
            json j;
            sina_today_t::to_json(j, t);
            LCL_INFO("\n{}", j.dump(4));
        }
    }

	return 0;
}