#include <libany/netstream/tcp.h>

using namespace ::libany::netstream;

TCPClientIOStream::TCPClientIOStream()
{
}

TCPClientIOStream::~TCPClientIOStream()
{
}

int TCPClientIOStream::read(void* p, int s)
{
	return _fd.recv(p, s, 0, 0, 0);
}

int TCPClientIOStream::write(const void* p, int s)
{
	return _fd.send(p, s, 0, 0);
}

bool TCPClientIOStream::eos()
{
	return false;
}

void TCPClientIOStream::connect(const char* host, int port)
{
	try {
		int error;
		_fd.connect(host, port, 0, &error);
	}
	catch(...) {
		throw;
	}
}

