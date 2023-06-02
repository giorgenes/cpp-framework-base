#include "fd.h"

using namespace ::libany::stdstream;

IOFDStream::IOFDStream(int in, int out)
	: _fd(in, out)
{
}

int IOFDStream::read(char* p, int s)
{
	int error;
	return _fd.read(p, s, 0, &error, 0);
}

bool IOFDStream::eos()
{
	return false;
}

int IOFDStream::write(const char*p , int s)
{
	int error;
	return _fd.write(p, s, 0, &error);
}


void IOFDStream::setfd(int in, int out)
{
	_fd.setfd(in, out);
}


void IOFDStream::close()
{
	_fd.close();
}

