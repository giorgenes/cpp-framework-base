#include "cut.h"

using namespace ::libany::stdstream;


int ICutStream::read(void* p, int len)
{
	/* don't try to read beyond the
	 * cut limit */
	if(_count + len > _max) {
		len = _max - _count;
	}

	int b = _st.read(p, len);
	if(b > 0) {
		_count += b;
	}

	return b;
}

bool ICutStream::eos()
{
	if(_count >= _max) {
		return true;
	}

	return _st.eos();
}


