#pragma once
#include <memory>
#include <assert.h>

namespace S4
{

//用于在智能指针释放时删除new对象
template<typename TYPE >
struct array_deleter {
	void operator ()(TYPE const * p)
	{
		delete[] p;
	}
};

//用智能指针管理new出来的指针
template<typename T>
class shared_array
{
private:

public:
	shared_array() noexcept:
		_sp(nullptr),
		_size(0)
	{
	}

	//new出一块新内存，并用智能指针管理
	explicit shared_array(size_t len) noexcept:
		_sp(new T[len], array_deleter<T>()),
		_size(len)
	{
	}

	//拷贝构造，智能指针增加引用
	// shared_array(const shared_array& cp) noexcept:
	// 	_sp(cp.sp()),
	// 	_size(cp.size())
	// {
	// }

	//原始指针
	T* get() noexcept{
		return _sp.get();
	};

	T* get() const noexcept{
		return _sp.get();
	}

	// //
	// std::shared_ptr<T> sp() noexcept{
	// 	return _sp;
	// };
	// //
	// std::shared_ptr<T> sp() const noexcept{
	// 	return _sp;
	// };

	T& operator[](size_t n) noexcept{
		assert(n < _size);
		return sp().get()[n];
	}

	const T& operator[](size_t n) const noexcept{
		return sp().get()[n];
	}

	const size_t size() noexcept{
		return _size;
	};

	const size_t size() const noexcept{
		return _size;
	};

	// shared_array& operator=(const shared_array& b) noexcept{
	// 	this->_size = b.size();
	// 	this->_sp = b.sp();
	// 	return *this;
	// }
private:

	std::shared_ptr<T> _sp;
	size_t _size;
};


typedef shared_array<char> char_array_t;

// byte_buffer_t make_byte_buffer(const char * pBuff, unsigned int len);

// struct shared_byte_buffer_t
// {
// 	byte_buffer_t buffer;
// 	size_t curLen;

// 	shared_byte_buffer_t(size_t buffer_size):
// 		buffer(buffer_size),
// 		curLen(0)
// 	{
// 	}

// 	char* get() {
// 		return buffer.sp().get();
// 	}

// 	char& operator[](size_t n) {
// 		assert(n < buffer.size());
// 		return buffer[n];
// 	}

// };

// int to_byte_buffer(char * pBuff, unsigned int len, shared_byte_buffer_t& byte_buffer);

}



