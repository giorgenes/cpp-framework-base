#include <libany/objectstream/stream.h>
#include <string.h>
#include <stdexcept>

using namespace ::libany::estp;
static const char g_magic[] = "e+";

#define STREAM_HEADER_SIZE (sizeof(_guid) + sizeof(g_magic))

#define STATE_WRITE (1 << 0)
#define STATE_READ  (1 << 1)
#define STATE_EOS   (1 << 2)

/* FIXME: optimize this */
int ObjectStream::write(const char* ptr, int s)
{
	init_write();
	
	if(s <= 0) return 0;

	if(!ptr) return -1;

	char buf[sizeof(s)+1] = "S";
	memcpy(buf+1, &s, sizeof(s));

	_ou.write(buf, sizeof(buf));
	_ou.write(ptr, s);

	return s;
}

int ObjectStream::read(char* ptr, int s)
{
	init_read();
	if(eos()) return 0;

	if(s <= 0) return 0;

	int r = _queue.read(ptr, s);
	s -= r;
	ptr += r;

	if(s == 0) return r;

	/* we fill the queue and then call us
	 * again to read again from the queue */
	char S;
	_ou.read(&S, 1);
	if(S == 'E') {
		_st |= STATE_EOS;
		return r;
	}
	if(S != 'S') {
		throw std::runtime_error("unexpected character in object stream");
	}
	int size;
	_ou.read((char*)&size, (int)sizeof(size));
	char buf[1024];
	int t;
	while(size > 0) {
		t = size > sizeof(buf) ? sizeof(buf) : size;
		t = _ou.read(buf, t);
		if(t <= 0) {
			throw std::runtime_error("unexpected end of stream in object stream");
		}
		_queue.write(buf, t);
		size -= t;
	}

	return r + ObjectStream::read(ptr, s); 
}

bool ObjectStream::eos()
{
	return _st & STATE_EOS;
}

void ObjectStream::init_read()
{
	if(_st & STATE_READ) return;

	char buf[STREAM_HEADER_SIZE];

	if(_complete_stream.read(buf, sizeof(buf)) <= 0) {
		// EOF
		_st |= STATE_EOS;
		return;
	}

	if(memcmp(buf, g_magic, sizeof(g_magic))) {
		// invalid magic/stream
		throw std::runtime_error("invalid magic at object stream");
	}

	_st |= STATE_READ;

	memcpy(&_guid, buf + sizeof(g_magic), sizeof(_guid));
}

void ObjectStream::init_write()
{
	if(_st & STATE_WRITE) return;

	char buf[STREAM_HEADER_SIZE];

	memcpy(buf, g_magic, sizeof(g_magic));
	memcpy(buf + sizeof(g_magic), &_guid, sizeof(_guid));

	_ou.write(buf, sizeof(buf));

	_st |= STATE_WRITE;
}

void ObjectStream::close()
{
	if(_st & STATE_WRITE) {
	}
}

ObjectStream::~ObjectStream()
{
}

