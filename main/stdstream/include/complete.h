#ifndef __libany__stdstream__complete_h
#define __libany__stdstream__complete_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class ICompleteStream : public ::libany::stream::IStream {
			private:
				::libany::stream::IStream* _st;
			public:
				ICompleteStream(::libany::stream::IStream*);

				int read(void*, int);
				bool eos();
		};
	}
}

#endif
