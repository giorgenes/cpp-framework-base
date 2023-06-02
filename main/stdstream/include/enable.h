#ifndef __libany__stdstream__enable_h
#define __libany__stdstream__enable_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class IEnableStream : public ::libany::stream::IStream {
			private:
				::libany::stream::IStream& _st;
				bool _enabled;
			public:
				IEnableStream(::libany::stream::IStream &st) 
					: _st(st), _enabled(false) {
				};
				int read(void*, int);
				bool eos();

				void set_enable(bool e) {
					_enabled = e;
				};

		};

		class IOEnableStream : public ::libany::stream::IOStream {
			private:
				::libany::stream::IOStream& _st;
				bool _enabled;
			public:
				IOEnableStream(::libany::stream::IOStream &st) 
					: _st(st), _enabled(false) {
				};
				int read(void*, int);
				bool eos();
				int write(const void*, int);

				void set_enable(bool e) {
					_enabled = e;
				};
		};
	}
}

#endif
