#include "queue.h"
#include <string.h>
#include <stdlib.h>

using namespace ::libany::stdstream;

int QueueStream::read(char* p, int s)
{
	if(s <= 0) return 0;
	if(eos()) return 0;

	int t;
	if(_e > _b) {
		t = _e - _b > s ? s : _e - _b;
		memcpy(p, _buf + _b, t);

		_b += t;

		return t;
	}
	else {
		t = _max - _b > s ? s : _max - _b;
		memcpy(p, _buf + _b, t);

		_b = 0;
		return t + QueueStream::read(((char*)p) + t, s - t);
	}

	return 0;
}

bool QueueStream::eos()
{
	return _b == _e;
}

void QueueStream::alloc_enougth_room(int s)
{
	if(size() + s > _max) {
		/* when resizing we rearrange the buffer
		 * inside the new buffer */
		char* buf = static_cast<char*>(malloc((_max + s) * 2));
		if(!buf) {
			/* FIXME: throw something */
			return;
		}
		_e = QueueStream::read(buf, size());
		_b = 0;
		_max = (_max + s) * 2;
		if(_buf) free(_buf);
		_buf = buf;
	}
}

int QueueStream::write(const char*p , int s)
{
	if(s <= 0) return 0;
	
	alloc_enougth_room(s);

	if(_e >= _b) {
		int t = s > _max - _e ? _max - _e : s;

		memcpy(_buf + _e, p, t);
		_e += t;
		p += t;
		s -= t;
		if(_e == _max) {
			_e = 0;
		}

		return t + QueueStream::write(p, s);
	}
	else {
		/* I'm sure we have enougth room */
		memcpy(_buf + _e, p, s);
		_e += s;

		return s;
	}

	return 0;
}


