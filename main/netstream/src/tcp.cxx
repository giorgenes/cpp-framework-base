#include <libany/netstream/tcp.h>

using namespace ::libany::netstream;

TCPClientStream::TCPClientStream()
{
}

TCPClientStream::~TCPClientStream()
{
}

int TCPClientStream::read(void* p, int s)
{
	return _fd.recv(p, s, 0, 0, 0);
}

int TCPClientStream::write(const void* p, int s)
{
	return _fd.send(p, s, 0, 0);
}

bool TCPClientStream::eos()
{
	return false;
}

void TCPClientStream::connect(const char* host, int port)
{
	try {
		int error;
		_fd.connect(host, port, 0, &error);
	}
	catch(...) {
		throw;
	}
}

