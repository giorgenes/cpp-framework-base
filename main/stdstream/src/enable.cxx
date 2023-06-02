#include "enable.h"

using namespace ::libany::stdstream;

int IOEnableStream::read(char* p, int s)
{
	if(_enabled) {
		return _st.read(p, s);
	}
	return 0;
}

bool IOEnableStream::eos()
{
	if(_enabled) {
		return _st.eos();
	}
	return true;
}

int IOEnableStream::write(const char*p , int s)
{
	if(_enabled) {
		return _st.write(p, s);
	}
	return 0;
}


