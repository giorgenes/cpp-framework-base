#include "buffer.h"
#include <stdlib.h>

namespace impl = ::libany::memory;

void impl::Buffer::set_size(int size)
{
	set_capacity(size);
	_size = size;
}

void impl::Buffer::set_capacity(int size)
{
	if(size > _max) {
		_max += size;
		_max *= 2;
		_buf = (char*)realloc(_buf, _max);
		if(_buf) {
			// TODO: sux
			return;
		}
	}
}

void impl::Buffer::grow(int size)
{
	set_size(_size + size);
}

void impl::Buffer::grow_capacity(int size)
{
	set_capacity(_size + size);
}


impl::Buffer::~Buffer()
{
	if(_buf) {
		free(_buf);
	}
}
