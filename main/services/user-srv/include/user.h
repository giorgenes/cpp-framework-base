#ifndef __libany__usersrv__user_h
#define __libany__usersrv__user_h

#include <libany/rcpp/service.h>

namespace libany {
	namespace usersrv {
		class UserSrv : public ::libany::rcpp::Service {
			public:
				::libany::rcpp::ClientHandler* new_client();
		};
	}
}

#endif
