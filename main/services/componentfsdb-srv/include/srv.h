#ifndef __libany__componentfsdbsrv__srv_h
#define __libany__componentfsdbsrv__srv_h

#include <libany/rcpp/service.h>

namespace libany {	
	namespace componentfsdbsrv {
		class DB;
		
		class ComponentDBSrv : public ::libany::rcpp::Service {
			private:
				DB* _db;
			public:
				::libany::rcpp::ClientHandler* new_client();

				ComponentDBSrv();
		};
	}
}

#endif
