#ifndef __libany__stfactory__uristream__h
#define __libany__stfactory__uristream__h

#include <libany/stream/stream.h>

namespace libany {
	namespace stfactory {
		class URIStream : public ::libany::stream::IOStream {
			private:
				::libany::stream::IOStream* _stream;
			public:
				URIStream(const char*);
				virtual bool eos();
				virtual int read(void*, int);
				virtual int write(const void*, int);
				virtual ~URIStream();
		};
	}
}

#endif
