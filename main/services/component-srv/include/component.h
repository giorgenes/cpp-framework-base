#ifndef __libany__componentsrv__component_h
#define __libany__componentsrv__component_h

#include <libany/rcpp/service.h>

namespace componentsrv_impl {
	class DB;
}

namespace libany {	
	namespace componentsrv {
		class ComponentSrv : public ::libany::rcpp::Service {
			private:
				::componentsrv_impl::DB* _db;
			public:
				::libany::rcpp::ClientHandler* new_client();

				ComponentSrv();
		};
	}
}

#endif
