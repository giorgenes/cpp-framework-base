#include <libany/stfactory/stfactory.h>
#include <libany/stfactory/uristream.h>
#include <stdexcept>

using namespace ::libany::stfactory;

URIStream::URIStream(const char* uri)
	: _stream(0)
{
	try {
		StreamFactory f;
		_stream = f.build(uri);

		if(!_stream) {
			throw std::runtime_error("could not create stream for URI");
		}
	}
	catch(...) {
		throw;
	}
}

bool URIStream::eos()
{
	return _stream->eos();
}

int URIStream::read(void* p, int s)
{
	return _stream->read(p, s);
}

int URIStream::write(const void* p, int s)
{
	return _stream->write(p, s);
}

URIStream::~URIStream()
{
	if(_stream) {
		delete _stream;
	}
}

