#include "common/s4logger.h"
#include "common/s4conf.h"

#include "jsonTypes/his_ifeng_raw_t.h"

CREATE_LOCAL_LOGGER("test_logger")


int main(int argc, char** argv)
{
	INFO("console only at begning.");
    
    his_ifeng_raw_t_tester();

	WARN("Going to take place of default logger setting");

	S4::glb_conf_ctx_t::logger_t p;
	p.save_path = "./logs";
	p.enable_file_all = true;
	S4::s4logger::pInstance()->init(&p);
	LCL_INFO("This message of local block show in ./logs/ S4xxx.log");

	p.enable_file_all_pure = true;
	S4::s4logger::pInstance()->init(&p);
	LCL_WARN("This message of local block show in ./logs/ S4xxx.log and S4xxx_pure.log");

	p.enable_file_err = true;
	S4::s4logger::pInstance()->init(&p);
	LCL_ERR("This message of local block show in ./logs/ S4xxx.log and S4xxx_pure.log and S4xxx_err.log");

	p.enable_file_err_pure = true;
	S4::s4logger::pInstance()->init(&p);
	ERR("This message of global show in ./logs/ S4xxx.log and S4xxx_pure.log and S4xxx_err.log and S4xxx_err_pure.log");

	LCL_FATAL("Program terminated by FATAL message.");
}