#include "null.h"

using namespace ::libany::stdstream;

int IONullStream::read(void* p, int s)
{
	p = p;
	s = s;
	return 0;
}

bool IONullStream::eos()
{
	return true;
}

int IONullStream::write(const void*p , int s)
{
	p = p;
	s = s;
	return 0;
}


