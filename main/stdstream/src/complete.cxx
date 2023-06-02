#include "complete.h"

using namespace ::libany::stdstream;

static int 
fullread(::libany::stream::Stream& _st, char* p, int len)
{
	int b = 0;
	int r;
	while(len > 0) {
		r = _st.read(p, len);
		if(r <= 0) {
			return r;
		}
		p = (char*)p + r;
		len -= r;
		b += r;
	}

	return b;
}

int CompleteStream::read(char* p, int len)
{
	return fullread(_st, p, len);
}

bool CompleteStream::eos()
{
	return _st.eos();
}

void CompleteStream::close()
{
	_st.close();
}

int CompleteStream::write(const char* p, int s)
{
	return _st.write(p, s);
}

