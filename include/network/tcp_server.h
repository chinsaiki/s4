//Base on asio:
// tcp_json_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "chat_message.h"
#include "common/s4json_util.h"
#include "common/s4exceptions.h"
#include "common/s4thread.h"

namespace S4{
namespace NW{
  
using asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<chat_message> chat_message_queue;
typedef std::deque<json_ptr_t> json_message_queue;

//----------------------------------------------------------------------

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void deliver(const chat_message& msg) = 0;
  virtual void close() = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------

class chat_room
{
public:
  size_t size(void) const{
    return participants_.size();
  }

  void join(unsigned short port, chat_participant_ptr participant)
  {
    if ( participants_.count(port) )
      participants_[port]->close();
    Locker l(_mux);
    participants_[port] = participant;
  }

  void leave(unsigned short port, chat_participant_ptr participant)
  {
    if( participants_.count(port) && participants_[port].get() == participant.get() ){
      Locker l(_mux);
      participants_.erase(port);
    }
  }

  void deliver(unsigned short port, const chat_message& msg)
  {
    json_ptr_t pJ = msg.body_json();
    if (pJ){
      (*pJ)["port"] = port;
      Locker l(_mux);
      _recv_msgs.push_back(pJ);
    }
  }

  bool read(json_ptr_t& pJ)
  {
    if( _recv_msgs.size() ){
      Locker l(_mux);
      pJ = _recv_msgs.front();
      _recv_msgs.pop_front();
      return true;
    }
    return false;
  }

  bool write(const json_ptr_t& pJ, unsigned short port)
  {
    Locker l(_mux);
    std::string s = pJ->dump();
    if (s.size() > chat_message::max_body_length)
    {
      throw network_Error("json str out of length");
    }

    chat_message msg;
    msg.body_length(s.size());
    std::memcpy(msg.body(), s.c_str(), s.size());
    msg.encode_header();

    if ( port == 0){
      for (auto& participant: participants_)
        participant.second->deliver(msg);
    }else{
      if (participants_.count(port)){
        participants_[port]->deliver(msg);
      }
    }
    return true;
  }

  std::set<unsigned short> participants(void)
  {
    std::set<unsigned short> ret;
    for (auto& participant: participants_)
      ret.insert(participant.first);

    return std::move(ret);
  }


private:
  std::map<unsigned short, chat_participant_ptr> participants_;
  json_message_queue _recv_msgs;
  // json_message_queue _send_msgs;

  Mutex _mux;
};

//----------------------------------------------------------------------

class chat_session
  : public chat_participant,
    public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)),
      room_(room)
  {
  }

  void start()
  {
    room_.join(socket_.remote_endpoint().port(), shared_from_this());
    do_read_header();
  }

  void close()
  {
    room_.leave(socket_.remote_endpoint().port(), shared_from_this());
    socket_.close();
  }

  void deliver(const chat_message& msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
    {
      do_write();
    }
  }

private:
  void do_read_header()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            room_.leave(socket_.remote_endpoint().port(), shared_from_this());
          }
        });
  }

  void do_read_body()
  {
    auto self(shared_from_this());
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            room_.deliver(socket_.remote_endpoint().port(), read_msg_);
            do_read_header();
          }
          else
          {
            room_.leave(socket_.remote_endpoint().port(), shared_from_this());
          }
        });
  }

  void do_write()
  {
    auto self(shared_from_this());
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this, self](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            room_.leave(socket_.remote_endpoint().port(), shared_from_this());
          }
        });
  }

  tcp::socket socket_;
  chat_room& room_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

//----------------------------------------------------------------------

class tcp_json_server
{
public:
  tcp_json_server(asio::io_context& io_context,
      const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
  {
    do_accept();
  }

  bool read(json_ptr_t& pJ)
  {
    return room_.read(pJ);
  }

  bool write_broadcast(const json_ptr_t& pJ)
  {
    return room_.write(pJ, 0);
  }

  bool write(const json_ptr_t& pJ, unsigned short port)
  {
    return room_.write(pJ, port);
  }

  std::set<unsigned short> clients(void)
  {
    return std::move(room_.participants());
  }

  size_t client_nb(void) const
  {
    return room_.size();
  }

  void stop() { _stop = true; }

private:
  void do_accept()
  {
    acceptor_.async_accept(
        [this](std::error_code ec, tcp::socket socket)
        {
          if (_stop) return;
          if (!ec)
          {
            std::cout << "accept" << std::endl;
            std::make_shared<chat_session>(std::move(socket), room_)->start();
          }
          do_accept();
        });
  }

  tcp::acceptor acceptor_;
  chat_room room_;

  bool _stop = false;
};

// class tcp_json_server_runner : public thread_runner_t
// {
// public:
//   tcp_json_server_runner(const tcp::endpoint& ep):
//     _endpoint(ep)
//   {

//   }
// private:
//   tcp::endpoint _endpoint;
// }



//----------------------------------------------------------------------

// int main(int argc, char* argv[])
// {
//   try
//   {
//     if (argc < 2)
//     {
//       std::cerr << "Usage: tcp_json_server <port> [<port> ...]\n";
//       return 1;
//     }

//     asio::io_context io_context;

//     std::list<tcp_json_server> servers;
//     for (int i = 1; i < argc; ++i)
//     {
//       tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
//       servers.emplace_back(io_context, endpoint);
//     }

//     io_context.run();
//   }
//   catch (std::exception& e)
//   {
//     std::cerr << "Exception: " << e.what() << "\n";
//   }

//   return 0;
// }

} // namespace NW
} // namespace S4
