#include "bufstream.h"
#include <string.h>

using namespace ::libany::stdstream;

/* we gotta read as much data
 * as possible, not only the data
 * in the buffer XOR the data in
 * the stream */
int BufStream::read(char* p, int len)
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

bool BufStream::eos()
{
	if(_p < &_buf[sizeof(_buf)]) {
		return false;
	}
	
	return _st.eos();
}

int BufStream::unread(const char* p, int len)
{
	if(_p - len < _buf) {
		return 0;
	}

	_p -= len;
	memcpy(_p, p, len);

	return len;
}

int BufStream::readline(char* s, size_t len)
{
	char* p = (char*)s;
	char buf[LIBANY_STDSTREAM_BUFSIZE];
	size_t t;
	int r;
	

	t = len > sizeof(buf) ? sizeof(buf) : len;

	if((r = BufStream::read(p, t-1)) <= 0) {
		return r;
	}

	p[r] = 0;
	int i = 0;
	while(i < r) {
		if(p[i] == '\n') {
			break;
		}
		i++;
	}
	if(i < r) {
		i++;
		unread(&p[i], r - i);
	}
	else {
		i = r;
	}
	p[i] = 0;

	return i;
}

int BufStream::write(const char* buf, int size)
{
	return _st.write(buf, size);
}
