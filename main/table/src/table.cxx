#include "table.h"
#include <stdlib.h>
#include <stdexcept>

namespace tbi = ::libany::table;

tbi::Table::Table()
{
}

tbi::Table::~Table()
{
}

void 
tbi::Table::build_from_stream(::libany::stream::Stream& in)
{
	int len;

	if(in.read(&_nfields, sizeof(_nfields)) <= 0) {
		throw std::runtime_error("broken stream");
	}
	
	if(in.read(&_ntuples, sizeof(_ntuples)) <= 0) {
		throw std::runtime_error("broken stream");
	}
	
	_buf.set_capacity(1024);
	while((len=in.read(_buf, 1024)) > 0) {
		_buf.grow(len);
		_buf.grow_capacity(1024);
	}

	//fprintf(stderr, "buf: %s\n", (const char*)_buf);
	char* b = _buf;
	unsigned int* idx;
	int n = 0;
	char* p = _buf;
	for(unsigned i=0; i<_buf.size(); i++) {
		if(p[i] == '|') {
			p[i] = 0;
			_fields.grow(sizeof(unsigned));
			idx = (unsigned int*)((void*)_fields);
			idx[n++] = b - _buf;
			//fprintf(stderr, "fields: %s\n", b);
			b = &p[i+1];
		}
	}
	//fprintf(stderr, "%dx%d\n", _ntuples, _nfields);
	if(n < _ntuples*_nfields + _nfields) {
		throw std::runtime_error("broken stream");
	}
}

const char* 
tbi::Table::getfield(int line, int pos) const
{
	if(pos >= _nfields) {
		return 0;
	}

	if(line >= _ntuples) {
		return 0;
	}

	const unsigned int* p = 
		(const unsigned int*)((const void*)_fields);
	return _buf + p[(line+1) * _nfields + pos];
}

const char* tbi::Table::field_name(int n) const
{
	return getfield(0, n);
}

const char* 
tbi::Table::field_value(int line, int pos) const
{
	return getfield(line, pos);
}

