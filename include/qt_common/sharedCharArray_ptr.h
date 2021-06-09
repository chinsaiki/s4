#pragma once

#include "qt_common/catch_ptr.h"
#include <assert.h>

namespace S4{

class sharedCharArray : public Catch::SharedImpl<Catch::IShared>
{
public:
    explicit sharedCharArray(size_t n):
        _p(new char[n]),
        _n(n)
    {}

    virtual ~sharedCharArray(){
        delete _p;
    }

    size_t size() const { return _n; }

    char * get() const { return _p; }

	char& operator[](size_t n) noexcept{
		assert(n < _n);
		return _p[n];
	}

	const char& operator[](size_t n) const noexcept{
		return _p[n];
	}

private:
    char * const _p;
    const size_t _n;
};

typedef Catch::Ptr<sharedCharArray> sharedCharArray_ptr;

inline
sharedCharArray_ptr make_sharedCharArray_ptr(size_t n) {
    return sharedCharArray_ptr(new sharedCharArray(n));
}

}