//
// async_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "common/s4conf.h"
#include "common/s4signalhandle.h"

#include <cstdlib>
#include <iostream>
#include <asio.hpp>
#include <functional>
#include <thread>
CREATE_LOCAL_LOGGER(testAsioUDP)

namespace asio {
	namespace placeholders {
		static decltype(::std::placeholders::_1)& error = ::std::placeholders::_1;
		static decltype(::std::placeholders::_2)& bytes_transferred = ::std::placeholders::_2;
		static decltype(::std::placeholders::_2)& iterator = ::std::placeholders::_2;
		static decltype(::std::placeholders::_2)& signal_number = ::std::placeholders::_2;
	}
}

using asio::ip::udp;

class server
{
public:
  server(asio::io_service& io_service, short port)
    : io_service_(io_service),
      socket_(io_service, udp::endpoint(udp::v4(), port))
  {
    socket_.async_receive_from(
        asio::buffer(data_, max_length), sender_endpoint_,
        std::bind(&server::handle_receive_from, this,
          asio::placeholders::error,
          asio::placeholders::bytes_transferred));
  }

  void handle_receive_from(const std::error_code& error,
      size_t bytes_recvd)
  {
    // if (!error && bytes_recvd > 0)
    // {
    //   socket_.async_send_to(
    //       asio::buffer(data_, bytes_recvd), sender_endpoint_,
    //       std::bind(&server::handle_send_to, this,
    //         asio::placeholders::error,
    //         asio::placeholders::bytes_transferred));
    // }
    // else
    // {
    //   socket_.async_receive_from(
    //       asio::buffer(data_, max_length), sender_endpoint_,
    //       std::bind(&server::handle_receive_from, this,
    //         asio::placeholders::error,
    //         asio::placeholders::bytes_transferred));
    // }

    if (!error && bytes_recvd > 0){
      _frame_cnt++;
      _byte_cnt+=bytes_recvd;
    }
    if (error) 
        return;
    socket_.async_receive_from(
        asio::buffer(data_, max_length), sender_endpoint_,
        std::bind(&server::handle_receive_from, this,
          asio::placeholders::error,
          asio::placeholders::bytes_transferred));
  }

  void handle_send_to(const std::error_code& /*error*/,
      size_t /*bytes_sent*/)
  {
    socket_.async_receive_from(
        asio::buffer(data_, max_length), sender_endpoint_,
        std::bind(&server::handle_receive_from, this,
          asio::placeholders::error,
          asio::placeholders::bytes_transferred));
  }

  size_t frame_cnt() { return _frame_cnt; }
  size_t byte_cnt() { return _byte_cnt; }

  void stop() {
      socket_.close();
  }

private:
  asio::io_service& io_service_;
  udp::socket socket_;
  udp::endpoint sender_endpoint_;
  enum { max_length = 1024 };
  char data_[max_length];

  size_t _frame_cnt = 0;
  size_t _byte_cnt = 0;
};

int main(int argc, char* argv[])
{
    S4::signalhandler_t::HookupHandler();
	S4::glb_conf::pInstance()->load("../json_template/glb_conf_ctx_t.json");
	S4::s4logger::pInstance()->init((void*)S4::glb_conf::pInstance()->pLogger());

	// if (argc != 2)
	// {
	//   std::cerr << "Usage: async_udp_echo_server <port>\n";
	//   return 1;
	// }

	  //short port = (short)atoi(argv[1]);
	short port = 8888;
	asio::io_service io_service;

	using namespace std; // For atoi.
	server s(io_service, port);
    auto f = [&]() {
        try
        {
            io_service.run();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    };
    std::thread t(f);
    t.detach();

    while (!S4::signalhandler_t::getSigint())
    {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
    s.stop();

	LCL_INFO("recv frame_cnt={}, byte_cnt={}", s.frame_cnt(), s.byte_cnt());

  return 0;
}