#include <libany/bxtp/document.h>
#include <string.h>
#include <stdexcept>

using namespace ::libany::bxtp;

ODocument::~ODocument()
{
	if(_wdepth > 0) {
		_bxtp.reset();
	}
}


int ODocument::write(const void* ptr, int s)
{
	return _bxtp.write(ptr, s);
}


void ODocument::begin(const char* tag)
{
	_bxtp.begin(tag);
	_wdepth++;
}

void ODocument::end()
{
	_bxtp.end();
	_wdepth--;
}


void ODocument::write_tag(const char* tag, const char* val, int len)
{
	begin(tag);
	if(val != 0) {
		if(len == 0) {
			len = strlen(val);
		}
		ODocument::write(val, len);
	}
	end();
}

bool ODocument::complete_depth(int depth)
{
	bool b = false;
	while(_wdepth > depth) {
		end();
		b = true;
	}
	return b;
}

