#include <libany/bxtp/document.h>
#include <string.h>
#include <stdexcept>

using namespace ::libany::bxtp;

int Document::write(const void* ptr, int s)
{
	return _bxtp.write(ptr, s);
}


void Document::begin(const char* tag)
{
	_bxtp.begin(tag);
	_wdepth++;
}

void Document::end()
{
	_bxtp.end();
	_wdepth--;
}


void Document::write_tag(const char* tag, const char* val, int len)
{
	begin(tag);
	if(val != 0) {
		if(len == 0) {
			len = strlen(val);
		}
		Document::write(val, len);
	}
	end();
}

bool Document::complete_depth(int depth)
{
	bool b = false;
	while(_wdepth > depth) {
		end();
		b = true;
	}
	return b;
}

