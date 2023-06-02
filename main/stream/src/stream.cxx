#include "stream.h"

using namespace ::libany::stream;

int Stream::copyto(Stream* ou)
{
	char buf[1024];
	int r;
	int t = 0;

	while(!eos()) {
		if((r = this->read(buf, sizeof(buf))) <= 0) {
			break;
		}
		ou->write(buf, r);
		t += r;
	}

	return t;
}

int Stream::copyfrom(Stream* in)
{
	return in->copyto(this);
}

void Stream::close()
{
}

# if 0
void IOStream::close()
{
	IOStream::close();
	IOStream::close();
}
#endif

Stream::~Stream()
{
}

