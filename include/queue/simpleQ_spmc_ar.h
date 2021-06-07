#pragma once

#include "simpleQ_mpmc_ar.h"

namespace S4 {

/*
signal-Producter/Multi-Consumer Auto-Recycle none-lock queue 生产者单线程，消费者可多线程

extInfoT : 自定义queue数据附加信息，可为数据长度等

simpleQ_spmc_ar_t<extInfoT>::queParticle_arPtr_t : queue对外接口读取的数据结构，包含内部存储数据结构指针
simpleQ_spmc_ar_t<extInfoT>::queParticle_t : queue内部存储的数据结构，包含extInfo和pBuffer
pBuffer  : queue数据内存，总长由构造时page_size指定

Producter(P)调用P_get_tryBest获取载体p(包含数据内存指针)，写入数据后p析构时将自动回收到queue内，供Consumer消费；
此后Consumer(C)通过调用B_try_get获取p，使用完数据后p析构时将自动回收到queue内，供下一Producter生产使用。

Producter接口：
    P_get_tryBest : 总能获取到内存，如果生产所需内存已消耗完，将自动增加内存分配
    P_get_timeout : 非阻塞式，如果生产所需内存已消耗完，将超时失败

    P_send : 可不使用

Consumor接口：
    C_recv   : 阻塞式，直到有需消费数据为止
    C_recv_timeout : 非阻塞式，如果无所需消费内存，将超时失败

    C_return : 可不使用
*/
template<class extInfoT>
class simpleQ_spmc_ar_t: public simpleQ_mpmc_ar_t<extInfoT>
{
public:
	simpleQ_spmc_ar_t(unsigned int init_depth, size_t page_size, bool all_memalign):
    	simpleQ_mpmc_ar_t<extInfoT>(init_depth, page_size, all_memalign)
    {
        //
        ctok_dataPool = std::make_shared<moodycamel::ConsumerToken>(*_dataPool);    //dataPool : Consumer=1, Producer=n
        ptok_dataPtoC = std::make_shared<moodycamel::ProducerToken>(*_dataPtoC);    //dataPtoC : Consumer=n, Producer=1
    }

	//生产者从数据池中获取数据，阻塞。
	virtual void P_get(queParticle_arPtr_t& p) override
	{
		queParticle_ptr_t pool_data;
		_dataPool->wait_dequeue(*ctok_dataPool, pool_data);
        p = std::make_shared<queParticle_ar_t<extInfoT>>(pool_data, _dataPtoC, ptok_dataPtoC);
    }

	//生产者从数据池中获取数据，阻塞timeout_us时间后，若数据池为空则返回false。
    virtual bool P_get_timeout(queParticle_arPtr_t& p, long long us) override
	{
		queParticle_ptr_t pool_data;
		if (_dataPool->wait_dequeue_timed(*ctok_dataPool, pool_data, us)) {
            p = std::make_shared<queParticle_ar_t<extInfoT>>(pool_data, _dataPtoC, ptok_dataPtoC);
			return true;
		}
		p = nullptr;
        return false;
    }

	//生产者从数据池中获取数据，若数据池为空则申请新内存，若申请失败则返回false。
	virtual bool P_get_tryBest(queParticle_arPtr_t& p) override
	{
        queParticle_ptr_t pool_data;
        if (_dataPool->try_dequeue(*ctok_dataPool, pool_data)) {
            p = std::make_shared<queParticle_ar_t<extInfoT>>(pool_data, _dataPtoC, ptok_dataPtoC);
            return true;
        }

		if (!init_dataPool(64)){
            p = nullptr;
			return false;
        }

		return P_get_tryBest(p);
	}

	//生产者传递数据进入队列
	virtual void P_send(queParticle_arPtr_t& p) override
    {
        p.reset();
    }

	//生产者批量传递数据进入队列
	//virtual void P_send_bulk(std::vector<queParticle_arPtr_t>& pv) override
 //   {
 //       std::vector<queParticle_ptr_t> pruducted_data;
	//	pruducted_data.reserve(pv.size());
 //       for (auto& p : pv){
	//		p->pHostQ = nullptr;
	//		pruducted_data.emplace_back(std::move(p->pQdata));
 //       }

 //       _dataPtoC->enqueue_bulk(pruducted_data.begin(), pv.size());
 //       pv.clear();
 //   }

	//消费者从队列中获取生产者输送的数据，阻塞。
	virtual void C_recv(queParticle_arPtr_t& p) override
	{
		queParticle_ptr_t pruducted_data;
		_dataPtoC->wait_dequeue(pruducted_data);
        p = std::make_shared<queParticle_ar_t<extInfoT>>(pruducted_data, _dataPool);
    }

	//消费者从队列中获取生产者输送的数据，非阻塞。
	virtual bool C_try_recv(queParticle_arPtr_t& p) override
	{
		queParticle_ptr_t pruducted_data;
		if (_dataPtoC->try_dequeue(pruducted_data)) {
			p = std::make_shared<queParticle_ar_t<extInfoT>>(pruducted_data, _dataPool);
			return true;
		}
		p = nullptr;
        return false;
    }

    virtual bool C_recv_timeout(queParticle_arPtr_t& p, long long us) override
	{
		queParticle_ptr_t pruducted_data;
		if (_dataPtoC->wait_dequeue_timed(pruducted_data, us)) {
			p = std::make_shared<queParticle_ar_t<extInfoT>>(pruducted_data, _dataPool);
			return true;
		}
		p = nullptr;
		return false;
	}

	virtual bool C_wait_getBulk_timeout(std::vector<queParticle_arPtr_t>& pv, size_t max_nb, long long us) override
	{
        std::vector<queParticle_ptr_t> pruducted_data(max_nb);
		pv.clear();
		if (_dataPtoC->wait_dequeue_bulk_timed(pruducted_data.begin(), max_nb, us)) {
            for (auto& QB : pruducted_data) {
                if (!QB)
                    break;;
                queParticle_arPtr_t p = std::make_shared<queParticle_ar_t<extInfoT>>(QB, _dataPool);
                pv.emplace_back(p);
            }
			return true;
		}
		return false;
	}

	//消费者使用完帧数据后归还数据池
    virtual void C_return(queParticle_arPtr_t& p) override
    {
        p.reset();
    }
	
	//virtual sQ_ptr_t get_dataPtoC() { return _dataPtoC; };
	//virtual sQ_ptr_t get_dataPool() { return _dataPool; };

	// virtual unsigned char* getSp(size_t N) { 
	// 	if (N >= _sp_pool.size())
	// 		return nullptr;
	// 	return _sp_pool[N].get(); 
	// }

// 	virtual size_t getPageSize()  override { return _page_size; }
// 	virtual unsigned int getDepth()  override { return _depth; }
// 	virtual size_t size_approx_PtoC() override { return _dataPtoC->size_approx(); };
// 	virtual size_t size_approx_CtoP() override { return _dataPool->size_approx(); };
// private:
//     const size_t _page_size;
//     const bool _all_memalign;
// 	unsigned int _depth = 0;

// 	queue_ptr_t _dataPool;
//     queue_ptr_t _dataPtoC;

//     std::mutex _mem_mux;
// 	std::vector<char_array_t> _mem_hoster;
private:
    // std::shared_ptr<moodycamel::ProducerToken> ptok_dataPool;
    std::shared_ptr<moodycamel::ConsumerToken> ctok_dataPool;
    std::shared_ptr<moodycamel::ProducerToken> ptok_dataPtoC;
    // std::shared_ptr<moodycamel::ConsumerToken> ctok_dataPtoC;

};

inline void simpleQ_spmc_ar_test() {
#if defined(__linux__)
#define C_P64 "l"
#else
#define C_P64 "I64"
#endif

    struct header_t
	{
		size_t length = 0;
		int64_t CONNID = 0;
    };

	typedef simpleQ_spmc_ar_t<header_t> nwQ_t;
	typedef simpleQ_spmc_ar_t<header_t>::queParticle_arPtr_t nwQ_data_arPtr_t;
    std::shared_ptr<nwQ_t> nwQ = std::make_shared<nwQ_t>(10, 2048, true);

	for (int i = 0; i < 100; ++i) {
		printf("#%d ================\n", i);
		//productor
        for (int m = 0; m < i; ++m) {
			{
				printf("   ----------- \n");
                nwQ_data_arPtr_t pData;
                printf("1. pending-frame / space = %" C_P64 "u / %" C_P64 "u, depth = %u\n", nwQ->size_approx_PtoC(), nwQ->size_approx_CtoP(), nwQ->getDepth());
                if (nwQ->P_get_tryBest(pData)) {
                    pData->pQdata->info.length = i;
                    for (int j = 0; j < i; ++j) {
                        pData->pQdata->pBuffer[j] = (unsigned char)j;
                    }
                    printf("2. pending-frame / space = %" C_P64 "u / %" C_P64 "u, depth = %u, adr=%" C_P64 "x, info.length=%" C_P64 "u\n", nwQ->size_approx_PtoC(), nwQ->size_approx_CtoP(), nwQ->getDepth(), (uint64_t)pData->pQdata->pBuffer, pData->pQdata->info.length);
                }
            }
			printf("3. pending-frame / space = %" C_P64 "u / %" C_P64 "u, depth = %u\n", nwQ->size_approx_PtoC(), nwQ->size_approx_CtoP(), nwQ->getDepth());
		}

        //consumer
		for (int n = 0; n < i; ++n) {
			printf("   ----------- \n");
			nwQ_data_arPtr_t pData;
            if (nwQ->C_try_recv(pData)) {
                printf("4. pending-frame / space = %" C_P64 "u / %" C_P64 "u, depth = %u, adr=%" C_P64 "x, info.length=%" C_P64 "u\n", nwQ->size_approx_PtoC(), nwQ->size_approx_CtoP(), nwQ->getDepth(), (uint64_t)pData->pQdata->pBuffer, pData->pQdata->info.length);
            }
            else {
                printf("4. [fail] pending-frame / space = %" C_P64 "u / %" C_P64 "u, depth = %u\n", nwQ->size_approx_PtoC(), nwQ->size_approx_CtoP(), nwQ->getDepth());
            }
        }
    }
}

}