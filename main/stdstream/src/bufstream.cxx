#include "bufstream.h"
#include <string.h>

using namespace ::libany::stdstream;

/* we gotta read as much data
 * as possible, not only the data
 * in the buffer XOR the data in
 * the stream */
int IBufStream::read(void* p, int len)
{
	int b = 0;
	int toread;
	
	/* read the buffer part */
	if(_p < &_buf[sizeof(_buf)] && len > 0) {
		toread = 
			len > &_buf[sizeof(_buf)] - _p ? &_buf[sizeof(_buf)] - _p : len;
		memcpy(p, _p, toread);
		b += toread;
		_p += toread;
	}

	len -= b;

	if(len > 0) {
		int c = _st.read(p, len);
		if(c > 0 ) {
			b += c;
		}
	}

	return b;
}

bool IBufStream::eos()
{
	if(_p < &_buf[sizeof(_buf)]) {
		return false;
	}
	
	return _st.eos();
}

int IBufStream::unread(const char* p, int len)
{
	if(_p - len < _buf) {
		return 0;
	}

	_p -= len;
	memcpy(_p, p, len);

	return len;
}

