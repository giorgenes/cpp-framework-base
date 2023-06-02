#ifndef __libany__stdstream__urlencode_h
#define __libany__stdstream__urlencode_h

#include <libany/stream/stream.h>
#include <libany/stdstream/bufstream.h>

namespace libany {
	namespace stdstream {
		class IUrlEncodeStream : public ::libany::stream::IStream {
			private:
				char _key[256];
				::libany::stdstream::IBufStream _st;
				bool _end;
			public:
				IUrlEncodeStream(::libany::stream::IStream& st)
					: _st(st), _end(false) {
				};
				int read(void*, int);
				bool eos();

				bool get_next_pair();
				inline const char* key() {
					return _key;
				};
		};
	}
}

#endif
