#ifndef __libany__postgresrv__postgre_h
#define __libany__postgresrv__postgre_h

#include <libany/rcpp/service.h>

namespace postgresrv_impl {
	class PostgreDB;
}

namespace libany {
	namespace postgresrv {
		class PostgreSrv : public ::libany::rcpp::Service {
			private:
				postgresrv_impl::PostgreDB* _db;
			public:
				::libany::rcpp::ClientHandler* new_client();

				PostgreSrv();
				~PostgreSrv();
		};
	}
}

#endif
