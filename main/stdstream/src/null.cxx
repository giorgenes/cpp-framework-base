#include "null.h"

using namespace ::libany::stdstream;

int IONullStream::read(char* p, int s)
{
	p = p;
	s = s;
	return 0;
}

bool IONullStream::eos()
{
	return true;
}

int IONullStream::write(const char*p , int s)
{
	p = p;
	s = s;
	return 0;
}


