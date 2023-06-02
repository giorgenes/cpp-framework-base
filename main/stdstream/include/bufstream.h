#ifndef __libany__stdstream__bufstream_h
#define __libany__stdstream__bufstream_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class IBufStream : public ::libany::stream::IStream {
			private:
				::libany::stream::IStream& _st;
				char _buf[1024];
				char* _p;
			public:
				IBufStream(::libany::stream::IStream &st)
					: _st(st) {
						_p = &_buf[sizeof(_buf)];
				};

				int read(void*, int);
				bool eos();

				int unread(const char*, int);
		};
	}
}

#endif
