#ifndef __libany__stdstream__bufstream_h
#define __libany__stdstream__bufstream_h

#include <libany/stream/stream.h>
#include <stdlib.h>

#define LIBANY_STDSTREAM_BUFSIZE 1024

namespace libany {
	namespace stdstream {
		class BufStream : public virtual ::libany::stream::Stream {
			private:
				::libany::stream::Stream& _st;
				char _buf[LIBANY_STDSTREAM_BUFSIZE];
				char* _p;
			public:
				BufStream(::libany::stream::Stream &st)
					: _st(st) {
						_p = &_buf[sizeof(_buf)];
				};

				int read(char*, int);
				bool eos();
				int readline(char*, size_t);
				virtual int write(const char*, int);

				int unread(const char*, int);
		};
	}
}

#endif
