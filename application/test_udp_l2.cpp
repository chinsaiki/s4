#include "common/s4conf.h"
#include "common/s4signalhandle.h"

#include "network/L2_udp_recver_th_native.h"

using namespace S4;
using namespace S4::NW;

CREATE_LOCAL_LOGGER(testUDPL2)



int main(int argc, char** argv)
{
	signalhandler_t::HookupHandler();
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

	std::shared_ptr<L2DataQ_t> pL2DataQ = std::make_shared<L2DataQ_t>(1024, 4096, true);

	L2_udp_recver_th_native L2recver(pL2DataQ);

	L2recver.start("0.0.0.0", 8888);

	auto proc = [&]() {
		L2Data_arPtr_t p;
		bool has_l2_data;
		uint64_t last_rpt_ms = 0;
		do {
			has_l2_data = pL2DataQ->C_recv_timeout(p, 500000);
			if (!has_l2_data)
				break;
			if (p->pQdata->info.type == L2DataType::STATS_DATA) {
				L2Stats_t* stats = (L2Stats_t*)p->pQdata->pBuffer;
				LCL_INFO("L2 udp recv stats @ {} ms, dlt={}:\n{}", p->pQdata->info.local_time_ms, p->pQdata->info.local_time_ms - last_rpt_ms, stats->toString());
				last_rpt_ms = p->pQdata->info.local_time_ms;
			}
		} while (1);
	};
	std::thread t(proc);

#if 1
	//do send dmy data
    int send_fd = SockUtil::bindUdpSock(0, "127.0.0.1");
	struct sockaddr addrDst;
	SockUtil::makeAddr(&addrDst, "127.0.0.1", 8888);//
	int send_frame_nb = 0;
	int send_byte_nb = 0;
	for (int n = 0; n < 4096; ++n) {
		for (int i = 64; i < 1500; i += 16) {

			std::string s(i, (char)('a' + (send_frame_nb++) % 26));
			send_byte_nb += sendto(send_fd, s.data(), (int)s.size(), 0, &addrDst, sizeof(struct sockaddr));
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	signalhandler_t::dummyInt();
#else
	// while(!signalhandler_t::getSigint());
#endif

	L2recver.stop();
	t.join();


	LCL_INFO("send frame_cnt = {}, byte_cnt = {}", send_frame_nb, send_byte_nb);

	LCL_INFO("L2 UDP test done!");

	return 0;
}
