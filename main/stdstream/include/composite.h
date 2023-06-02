#ifndef __libany__stdstream__composite_h
#define __libany__stdstream__composite_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class IOCompositeStream : public ::libany::stream::IOStream {
			private:
				::libany::stream::IStream &_in;
				::libany::stream::OStream &_ou;
			public:
				IOCompositeStream(
						::libany::stream::IStream &in,
						::libany::stream::OStream &ou)
					: _in(in), _ou(ou) {
				};
				int read(void*, int);
				bool eos();
				int write(const void*, int);
		};
	}
}

#endif
