#include "complete.h"

using namespace ::libany::stdstream;

ICompleteStream::ICompleteStream(::libany::stream::IStream* st)
	: _st(st)
{
}

int ICompleteStream::read(void* p, int len)
{
	int b = 0;
	int r;
	while(len > 0) {
		r = _st->read(p, len);
		if(r <= 0) {
			return r;
		}
		p = (char*)p + r;
		len -= r;
		b += r;
	}

	return b;
}

bool ICompleteStream::eos()
{
	return _st->eos();
}


