#pragma once

#include "queue/shared_array.h"

#include <vector>
#include <map>

namespace S4 {


//生产者-消费者间的数据队列接口类，应线程安全
//P:生产者   C:消费者
//使用过程：
//  1.初始化时将构造以page_size为单位大小的帧数据池
//  2.生产者从数据池中获取数据：P_get
//  3.生产者将需要传递的数据写入帧数据，并通过队列发送给消费者：P_send
//  4.消费者从队列中取出帧数据：C_recv
//  5.消费者使用完帧数据后归还数据池：C_return
template<class queParticle_ptr_t>
class simpleQ_t
{
public:
	//生产者从数据池中获取数据，阻塞。
	virtual void P_get(queParticle_ptr_t& p) = 0;
	//生产者从数据池中获取数据，阻塞timeout_us时间后，若数据池为空则返回false。
	virtual bool P_get_timeout(queParticle_ptr_t& p, long long timeout_us) = 0;
	//生产者从数据池中获取数据，若数据池为空则申请新内存，若申请失败则返回false。
	virtual bool P_get_tryBest(queParticle_ptr_t& p) = 0;

	//生产者传递数据进入队列
	virtual void P_send(queParticle_ptr_t& p) = 0;
	//生产者批量传递数据进入队列
	virtual void P_send_bulk(std::vector<queParticle_ptr_t>& p) = 0;

	//消费者从队列中获取生产者输送的数据，阻塞。
	virtual void C_recv(queParticle_ptr_t& p) = 0;
	//消费者从队列中获取生产者输送的数据，非阻塞。
	virtual bool C_recv_try(queParticle_ptr_t& p) = 0;
	//消费者从队列中获取生产者输送的数据，阻塞timeout_us时间后，若队列为空则返回false。
	virtual bool C_recv_timeout(queParticle_ptr_t& p, long long us) = 0;
	//消费者从队列中批量获取生产者输送的数据，阻塞timeout_us时间后，若队列为空则返回false。
	virtual bool C_getBulk_timeout(std::vector<queParticle_ptr_t>& pv, size_t max_nb, long long us) = 0;

	//消费者使用完帧数据后归还数据池
	virtual void C_return(queParticle_ptr_t& p) = 0;

	//帧数据最大空间
	virtual size_t getPageSize() = 0;
	//数据池最大深度
	virtual unsigned int getDepth() = 0;

	//队列中未消费的数据帧数目
	virtual size_t size_approx_PtoC() = 0;
	//数据池中剩余可用于生产的数据帧数目
	virtual size_t size_approx_CtoP() = 0;
};


// simpleQ_ptr_t make_simpleQ_spsc_ptr(unsigned int depth, unsigned int page_size);
// simpleQ_ptr_t make_simpleQ_spmc_ptr(unsigned int depth, size_t page_size);

}