#include "common/s4conf.h"
#include "network/sockutil.h"
#include <thread>

using namespace S4;

CREATE_LOCAL_LOGGER(testUDP)

int main(int argc, char** argv)
{
	glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	s4logger::pInstance()->init((void*)glb_conf::pInstance()->pLogger());

    int recv_fd = SockUtil::bindUdpSock(10000, "127.0.0.1");
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
	SockUtil::makeAddr(&addrDst, "127.0.0.1", 10000);//UDP数据发送地址

	std::string s("this is a UDP test");
	sendto(send_fd, s.data(), (int)s.size(), 0, &addrDst, sizeof(struct sockaddr));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	t.join();

	LCL_INFO("UDP test done!");

	return 0;
}
