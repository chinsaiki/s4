
#include "queue/simpleQ.h"
// #include "simpleQ_spsc.h"
// #include "queue/simpleQ_mpmc.h"

namespace S4 {

simpleQ_t::simpleQ_t(size_t page_size) :
	_page_size(page_size)
{
}

bool simpleQ_t::malloc_particle(std::vector<queParticle_ptr_t>& particles, unsigned int num)
{
	if (num == 0)
		return true;

	size_t page_size = _page_size * num;
	char_array_t mem(page_size);
	if(mem.get() == nullptr)	//new fail
		return false;

	particles.clear();
	for(unsigned int n = 0; n < num; ++n){
		queParticle_ptr_t p(new simpleQueParticle_t(mem.get() + n * _page_size));
		// p->pBuffer = mem.get() + n * _page_size;
		particles.emplace_back(p);
	}

	{
		std::lock_guard<std::mutex> lk(_mem_mux);
		_mem_hoster.emplace_back(mem);
		// _mem_scope_list[mem.get()] = page_size;
		_depth += num;
	}

	return true;
}

// bool simpleQ_t::isInMem(unsigned char * ptr)
// {
// 	if(ptr == nullptr)
// 		return false;

// 	for(auto& mem_info:_mem_scope_list){
// 		if(ptr >= mem_info.first && ptr < mem_info.first + mem_info.second){
// 			return true;
// 		}
// 	}
// 	return false;
// }


// simpleQ_ptr_t make_simpleQ_spsc_ptr(unsigned int depth, unsigned int page_size)
// {
// 	return std::make_shared<simpleQ_spsc_t>(depth, page_size);
// }
// simpleQ_ptr_t make_simpleQ_mpmc_ptr(unsigned int depth, size_t page_size)
// {
// 	return std::make_shared<simpleQ_mpmc_t>(depth, page_size);
// }

}