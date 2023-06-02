#include <libany/bxtp/stream.h>
#include <string.h>
#include <stdexcept>

using namespace ::libany::bxtp;

void BxtpStream::reset()
{
	char buf[2] = "R";
	_ou.write(buf, 2);
}

int BxtpStream::write(const void* ptr, int s)
{
	int t;
	char buf[2] = "D";
	const char* p = static_cast<const char*>(ptr);
	
	while(s > 0) {
		t = s > 256 ? 256 : s;
		buf[1] = t;
		_ou.write(buf, 2);
		_ou.write(p, t);
		p += t;
		s -= t;
	}

	return s;
}


void BxtpStream::begin(const char* tag)
{
	unsigned int len = strlen(tag);
	if(len > 256) {
		len = 256;
	}

	char buf[256+2];

	buf[0] = 'T';
	buf[1] = len;
	strncpy(buf+2, tag, len);
	if(_ou.write(buf, len+2) <= 0) {
		throw std::runtime_error("could not write to stream");
	}
}

void BxtpStream::end()
{
	char buf[2] = "T";

	_ou.write(buf, 2);
}

