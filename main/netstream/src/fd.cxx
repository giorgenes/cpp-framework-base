#include <libany/netstream/fd.h>

using namespace ::libany::netstream;

FDStream::~FDStream()
{
}

int FDStream::read(void* p, int s)
{
	int error;
	return FDStream::fd.read(p, s, 0, &error, 0);
}

int FDStream::write(const void* p, int s)
{
	int error;
	return FDStream::fd.write(p, s, 0, &error);
}

bool FDStream::eos()
{
	return false;
}

