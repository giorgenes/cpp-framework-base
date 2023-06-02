#include "table.h"
#include <cstring>

namespace tbi = ::libany::table;

tbi::TableWriter::TableWriter(::libany::stream::Stream& stream, int nf, int nt)
	: _stream(stream), _nfields(nf), _ntuples(nt), _count(0) 
{
	_stream.write(&nf, sizeof(nf));
	_stream.write(&nt, sizeof(nt));
}


// TODO: e se tiver | e \n no meio da string? 
void tbi::TableWriter::write_field_name(const char* name)
{
	int len = std::strlen(name);
	_stream.write(name, len);
	_stream.write("|", 1);
}

void tbi::TableWriter::write_field_value(const char* value)
{
	write_field_name(value);
}
