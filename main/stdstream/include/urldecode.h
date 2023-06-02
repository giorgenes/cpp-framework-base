#ifndef __libany__stdstream__urldecode_h
#define __libany__stdstream__urldecode_h

#include <libany/stream/stream.h>
#include <libany/stdstream/bufstream.h>

namespace libany {
	namespace stdstream {
		class UrlDecodeStream : public virtual ::libany::stream::Stream {
			private:
				char _key[256];
				::libany::stdstream::BufStream _st;
				bool _end;
			public:
				UrlDecodeStream(::libany::stream::Stream& st)
					: _st(st), _end(false) {
				};
				int read(char*, int);
				bool eos();

				bool get_next_pair();
				inline const char* key() {
					return _key;
				};
		};
	}
}

#endif
