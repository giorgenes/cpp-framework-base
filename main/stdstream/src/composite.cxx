#include "composite.h"

using namespace ::libany::stdstream;

int IOCompositeStream::read(void* p, int s)
{
	return _in.read(p, s);
}

bool IOCompositeStream::eos()
{
	return _in.eos();
}

int IOCompositeStream::write(const void*p , int s)
{
	return _ou.write(p, s);
}


