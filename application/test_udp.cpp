#include "common/s4conf.h"
#include "common/s4signalhandle.h"

#include "network/sockutil.h"
#include <thread>
#include "queue/simpleQ_spmc_ar.h"

using namespace S4;

CREATE_LOCAL_LOGGER(testUDP)

void sr_test();
void burst_recv_test();

void queue_test();

int main(int argc, char** argv)
{
	signalhandler_t::HookupHandler();
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

	//simpleQ_spmc_ar_test();

	queue_test();

	// sr_test();

	// burst_recv_test();

	LCL_INFO("UDP test done!");

	return 0;
}

//send recv
void sr_test()
{
    int recv_fd = SockUtil::bindUdpSock(8888, "127.0.0.1");
    int send_fd = SockUtil::bindUdpSock(0, "127.0.0.1");
    LCL_INFO("recv_fd = {}, send_fd = {}", recv_fd, send_fd);

	auto recver = [&]() {
		char data[2048];
		int n = 0;
		do {
			n = recv(recv_fd, data, sizeof(data), 0);
			if (n > 0) {
				data[n] = 0;
				LCL_INFO("recv OK size = {}, data= {}", n, data);
			}
		} while (n == -1);
	};

	std::thread t(recver);

	struct sockaddr addrDst;
	SockUtil::makeAddr(&addrDst, "127.0.0.1", 8888);//

	std::string s("this is a UDP test");
	sendto(send_fd, s.data(), (int)s.size(), 0, &addrDst, sizeof(struct sockaddr));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	t.join();
	LCL_INFO("UDP sr_test done!");
}

void burst_recv_test()
{
    int recv_fd = SockUtil::bindUdpSock(8888);
    LCL_INFO("recv_fd = {}", recv_fd);
	char data[2048];
	int n = 0;

	size_t frame_cnt = 0;
	size_t byte_cnt = 0;
	while (!signalhandler_t::getSigint())
	{
		n = recv(recv_fd, data, sizeof(data), 0);
		if (n > 0) {
			frame_cnt++;
			byte_cnt+=n;
		}
	}
	
	LCL_INFO("recv frame_cnt={}, byte_cnt={}", frame_cnt, byte_cnt);

	LCL_INFO("UDP burst_recv_test done!");
}

void queue_test()
{

	struct udpDataInfo_t{
		int recv_fd = -1;
		int data_len = 0;
		//remote ip/port..
	};

	
	typedef simpleQ_spmc_ar_t<udpDataInfo_t> udpDataQ_t;
	typedef simpleQ_spmc_ar_t<udpDataInfo_t>::queParticle_arPtr_t udpData_arPtr_t;

	//data queue between recv & proc
	udpDataQ_t udpDataQ(16, 2048, true);
	size_t recv_frame_cnt = 0;
	size_t recv_byte_cnt = 0;
	size_t proc_frame_cnt = 0;
	size_t proc_byte_cnt = 0;

    int recv_fd = SockUtil::bindUdpSock(8888, "127.0.0.1");
    int send_fd = SockUtil::bindUdpSock(0, "127.0.0.1");
    LCL_INFO("recv_fd = {}, send_fd = {}", recv_fd, send_fd);

	//recv thread
	auto recver = [&]() {
		int n = 0;
		udpData_arPtr_t pData;
		udpDataQ.P_get_tryBest(pData);	//空数据会在析构时进入数据队列、被C收到，此时通过fd=-1,len=0判断是无效数据。
		std::vector<udpData_arPtr_t> bulk;
		bulk.reserve(64);
		do {
			n = recv(recv_fd, pData->pQdata->pBuffer, (int)udpDataQ.getPageSize(), 0);
			if (n > 0) {
				recv_frame_cnt++;
				recv_byte_cnt+=n;
				pData->pQdata->info.data_len = n;
				pData->pQdata->info.recv_fd = recv_fd;
				//bulk.emplace_back(std::move(pData));
				udpDataQ.P_get_tryBest(pData);
				// LCL_INFO("recv OK size = {}, data= {}", n, data);
			}else if (n!=-1){
				LCL_ERR("recv_fd broken!");
				break;
			}
			//else {
			//	if (bulk.size()>=64)
			//		udpDataQ.P_send_bulk(bulk);
			//}
		} while (!signalhandler_t::getSigint());
	};

	//proc thread
	auto procer = [&](){
		udpData_arPtr_t pData;
		bool got_data;
		do{
			got_data = udpDataQ.C_recv_timeout(pData, 100);
			if (got_data && pData->pQdata->info.recv_fd != -1){
				pData->pQdata->pBuffer[pData->pQdata->info.data_len] = 0;
				//LCL_INFO("proc #{} data={}", proc_frame_cnt, pData->pQdata->pBuffer);
				proc_frame_cnt++;
				proc_byte_cnt += pData->pQdata->info.data_len;
			}
		} while (!signalhandler_t::getSigint());
	};

	//start procer & recver
	std::thread tp(procer);
	std::thread tr(recver);

	//do send
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

	//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	signalhandler_t::dummyInt();

	tr.join();
	tp.join();

	LCL_INFO("send frame_cnt = {}, byte_cnt = {}", send_frame_nb, send_byte_nb);
	LCL_INFO("recv frame_cnt = {}, byte_cnt = {}", recv_frame_cnt, recv_byte_cnt);
	LCL_INFO("proc frame_cnt = {}, byte_cnt = {}", proc_frame_cnt, proc_byte_cnt);
	LCL_INFO("PtoC space = {} / {}, CtoP space = {}", udpDataQ.size_approx_PtoC(), udpDataQ.getDepth(), udpDataQ.size_approx_CtoP());
}