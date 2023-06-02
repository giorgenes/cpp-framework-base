#include "filestream.h"
#include <stdexcept>

using namespace ::libany::stdstream;

IOFileStream::IOFileStream(const char* path)
{
	IOFileStream(path, "r");
}

IOFileStream::IOFileStream(const char* path, const char* mode)
	: _owner(true)
{
	_fp = fopen(path, mode);
	if(!_fp) {
		throw std::runtime_error("could not open file");
	}
}

IOFileStream::IOFileStream(FILE* fp)
	: _fp(fp), _owner(false)
{
}


IOFileStream::~IOFileStream()
{
	if(_fp && _owner) {
		fclose(_fp);
	}
}

int IOFileStream::read(char* p, int len)
{
	return fread(p, sizeof(char), len, _fp);
}

bool IOFileStream::eos()
{
	return feof(_fp);
}

int IOFileStream::write(const char* p, int len)
{
	int r = fwrite(p, sizeof(char), len, _fp);
	if(r == 0) {
		throw std::runtime_error("could not write to file");
	}

	return r;
}

