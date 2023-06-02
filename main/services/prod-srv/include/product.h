#ifndef __libany__productsrv__product_h
#define __libany__productsrv__product_h

#include <libany/rcpp/service.h>

namespace libany {
	namespace productsrv {
		class ProductSrv : public ::libany::rcpp::Service {
			public:
				::libany::rcpp::ClientHandler* new_client();
		};
	}
}

#endif
