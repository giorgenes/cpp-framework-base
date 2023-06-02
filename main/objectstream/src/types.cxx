#include "types.h"

namespace ob = ::libany::estp;
namespace st = ::libany::stream;

st::Stream& ob::operator << (st::Stream& ou, int val)
{
	ObjectStream s(ou);

	s.write((const char*)&val, sizeof(val));

	return ou;
}

st::Stream& ob::operator >> (st::Stream& in, int& val)
{
	ObjectStream s(in);

	s.read((char*)&val, sizeof(val));

	return in;
}

