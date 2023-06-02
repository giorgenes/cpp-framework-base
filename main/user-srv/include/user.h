#ifndef __libany__usersrv__user_h
#define __libany__usersrv__user_h

#include <libany/rcpp/service.h>

namespace libany {
	namespace usersrv {
		class User : public ::libany::rcpp::Service {
			public:
				::libany::rcpp::Client* new_client();
		};
	}
}

#endif
