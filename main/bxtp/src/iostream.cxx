#include <libany/bxtp/stream.h>
#include <string.h>
#include <stdexcept>

using namespace ::libany::bxtp;

BxtpIOStream::~BxtpIOStream()
{
}

int BxtpIOStream::read(void* ptr, int s)
{
	return BxtpIStream::read(ptr, s);
}

int BxtpIOStream::write(const void* ptr, int s)
{
	return BxtpOStream::write(ptr, s);
}

bool BxtpIOStream::eos()
{
	return BxtpIStream::eos();
}

