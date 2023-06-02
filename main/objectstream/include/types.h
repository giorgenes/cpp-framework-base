#ifndef __libany__estp__type_h
#define __libany__estp__type_h

#include <libany/objectstream/stream.h>

#define ESTP_STREAM_TYPE_STREAM 0
#define ESTP_STREAM_TYPE_INT    1
#define ESTP_STREAM_TYPE_STRING 2

namespace libany {
	namespace estp {
		::libany::stream::Stream& operator << (::libany::stream::Stream&, int); 
		::libany::stream::Stream& operator >> (::libany::stream::Stream&, int&); 
	}
}

#endif
