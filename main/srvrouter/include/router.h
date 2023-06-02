#ifndef __libany__srvrouter__router_h
#define __libany__srvrouter__router_h

#include <libany/ios/ios.h>
#include <vector>

namespace libany {
	namespace srvrouter {
		class ServiceItem {
			public:
				char name[128];
				char address[1024];
		};

		class Router : public ::libany::ios::Router {
			private:
				std::vector<ServiceItem> _services;
				ServiceItem _auxitem;
				int _idx;
			public:
				Router() : _idx(-1) {};
				
				virtual const char* route(
					 const char* service, 
					 const char* key);

				virtual void search(const char* srv, const char* key);
				virtual const char* next();
				
				void configure(const char*, const char*);

				virtual ~Router();
		};
	}
}

#endif
