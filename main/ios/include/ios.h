#ifndef __libany__ios_h
#define __libany__ios_h

#include <libany/stream/stream.h>

namespace libany {
	namespace ios {
#if 0
		/* router class.
		 * Given a service name and
		 * a key, it returns a
		 * stream connected to that service */
		class Router {
			public:
				virtual const char* route(
					 const char* service, 
					 const char* key) = 0;
				
				virtual void search(const char* srv, const char* key) = 0;
				virtual const char* next() = 0;

				virtual ~Router();

		};
#endif

		class Client {
			protected:
				::libany::stream::Stream& _stream;
			public:
				Client(::libany::stream::Stream& r) : _stream(r) {
				};
		};
		
		class Service {
			public:
				virtual void handle_client(::libany::stream::Stream*) = 0;

				virtual ~Service();
		};
	}
}

#endif
