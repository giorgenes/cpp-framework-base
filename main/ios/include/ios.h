#ifndef __libany__ios_h
#define __libany__ios_h

#include <libany/stream/stream.h>

namespace libany {
	namespace ios {
		/* router class.
		 * Given a service name and
		 * a key, it returns a
		 * stream connected to that service */
		class Router {
			public:
				virtual const char* route(
					 const char* service, 
					 const char* key) = 0;
		};

		class Client {
			protected:
				Router& _router;
			public:
				Client(Router& r) : _router(r) {
				};
		};
		
		class Service {
			public:
				virtual void handle_client(::libany::stream::IOStream*) = 0;
		};
	}
}

#endif
