#include "strfile.h"
#include <cstdlib>
#include <stdexcept>
#include <cstring>
#include <libany/except/except.h>
#include "ftype.h"

namespace impl = ::libany::strfile;

// FIXME: optimize this STUPID implementation.

impl::StrFile::StrFile()
	: __str(0), __fp(0), __keyidx(0)
{
	__h.n = 0;
}

void impl::StrFile::loadstr(const char* key)
{
	unsigned int i = 0;
	unsigned int size;
	char buf[1024];
	
	for(;i<__h.n; i++) {
		std::fseek(__fp, __keyidx[i] + __h.keyb, SEEK_SET);
		std::fread(&size, sizeof(size), 1, __fp);
		std::fread(buf, 1, size, __fp);
		buf[size] = 0;

		//fprintf(stderr, "%s, %s\n", buf, key);
		if(std::strcmp(buf, key)==0) {
			std::fseek(__fp, __dataidx[i] + __h.datab, SEEK_SET);
			std::fread(&size, sizeof(size), 1, __fp);
			__str = static_cast<char*>(realloc(__str, size+1));
			std::fread(__str, 1, size, __fp);
			__str[size] = 0;
			//fprintf(stderr, "%s\n", __str);
			return;
		}
	}

	throw std::runtime_error("could not find key");
}


void impl::StrFile::load(const char* path)
{
	__fp = std::fopen(path, "r");
	if(!__fp) {
		throw ::libany::except::Errno();
	}
	
	std::fread(&__h, sizeof(__h), 1, __fp);
	
	__keyidx = static_cast<unsigned*>(
			std::malloc(sizeof(unsigned) * __h.n * 2));

	__dataidx = __keyidx + __h.n;

	std::fread(__keyidx, sizeof(unsigned), __h.n, __fp);
	std::fread(__dataidx, sizeof(unsigned), __h.n, __fp);
}

