#ifndef __libany__stdstream__null_h
#define __libany__stdstream__null_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class IONullStream : public ::libany::stream::IOStream {
			private:
			public:
				int read(void*, int);
				bool eos();
				int write(const void*, int);
		};
	}
}

#endif
