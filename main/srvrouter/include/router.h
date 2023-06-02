#ifndef __libany__srvrouter__router_h
#define __libany__srvrouter__router_h

#include <libany/ios/ios.h>

namespace libany {
	namespace srvrouter {
		class Router : public ::libany::ios::Router {
			public:
				virtual const char* route(
					 const char* service, 
					 const char* key);
				void configure(const char*, const char*);
		};
	}
}

#endif
