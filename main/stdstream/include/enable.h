#ifndef __libany__stdstream__enable_h
#define __libany__stdstream__enable_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class IOEnableStream : public virtual ::libany::stream::Stream {
			private:
				::libany::stream::Stream& _st;
				bool _enabled;
			public:
				IOEnableStream(::libany::stream::Stream &st) 
					: _st(st), _enabled(false) {
				};
				int read(char*, int);
				bool eos();
				int write(const char*, int);

				void set_enable(bool e) {
					_enabled = e;
				};
		};
	}
}

#endif
