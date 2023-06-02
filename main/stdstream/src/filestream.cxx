#include "filestream.h"
#include <stdexcept>

using namespace ::libany::stdstream;

IFileStream::IFileStream(const char* path)
	: _owner(true)
{
	_fp = fopen(path, "r");
	if(!_fp) {
		throw std::runtime_error("could not open file");
	}
}

IFileStream::IFileStream(FILE* fp)
	: _fp(fp), _owner(false)
{
}


IFileStream::~IFileStream()
{
	if(_fp && _owner) {
		fclose(_fp);
	}
}

int IFileStream::read(void* p, int len)
{
	return fread(p, sizeof(char), len, _fp);
}

bool IFileStream::eos()
{
	return feof(_fp);
}

OFileStream::OFileStream(const char* path)
{
	_fp = fopen(path, "w");
	if(!_fp) {
		throw std::runtime_error("could not open file for writing");
	}
	_owner = true;
}

OFileStream::OFileStream(FILE* fp)
{
	_fp = fp;
	_owner = false;
}

int OFileStream::write(const void* p, int len)
{
	return fwrite(p, sizeof(char), len, _fp);
}

OFileStream::~OFileStream()
{
	if(_fp && _owner) {
		fclose(_fp);
	}
}

