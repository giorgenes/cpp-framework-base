#ifndef __libany__stdstream__null_h
#define __libany__stdstream__null_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class IONullStream : public virtual ::libany::stream::Stream {
			private:
			public:
				int read(char*, int);
				bool eos();
				int write(const char*, int);
		};
	}
}

#endif
