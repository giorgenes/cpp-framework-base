#include <libany/stream/stream.h>

using namespace ::libany::stream;

int IStream::copyto(OStream* ou)
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

int OStream::copyfrom(IStream* in)
{
	return in->copyto(this);
}

