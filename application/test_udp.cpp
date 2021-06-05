#include "common/s4conf.h"
#include "common/s4signalhandle.h"

#include "network/sockutil.h"
#include <thread>

using namespace S4;

CREATE_LOCAL_LOGGER(testUDP)

void sr_test();
void burst_recv_test();

int main(int argc, char** argv)
{
	signalhandler_t::HookupHandler();
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());


	sr_test();

	burst_recv_test();

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