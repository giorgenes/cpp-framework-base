#include <libany/stfactory/stfactory.h>
#include <libany/netstream/tcp.h>
#include <string.h>
#include <stdexcept>
#include <memory>

using namespace ::libany::stfactory;

::libany::stream::IOStream* StreamFactory::build(const char* uri)
{
	char addr[1024];
	if(strncmp(uri, "tcp://", 6)==0) {
		uri += 6;
		strncpy(addr, uri, sizeof(addr));
		addr[sizeof(addr)-1] = 0;
		char* p = strchr(addr, ':');
		if(!p) {
			throw std::runtime_error("could not found : on uri");
		}
		*p++ = 0;
		int port = atoi(p);

		::libany::netstream::TCPClientIOStream* st = 0;

		try {
			st = new ::libany::netstream::TCPClientIOStream();
			st->connect(addr, port);
		}
		catch(...) {
			if(st) {
				delete st;
				st = 0;
			}
			throw;
		}
		return st;
	}
	return 0;
}

