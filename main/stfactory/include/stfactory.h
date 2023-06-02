#ifndef __libany__stfactory_stfactory__h
#define __libany__stfactory_stfactory__h

#include <libany/stream/stream.h>

namespace libany {
	namespace stfactory {
		class StreamFactory  {
			public:
				::libany::stream::Stream* build(const char*);
		};
	}
}

#endif
