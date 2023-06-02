#ifndef __libany__usersrvcli__user_h
#define __libany__usersrvcli__user_h

#include <libany/ios/ios.h>
#include <libany/stfactory/uristream.h>
#include <libany/bxtp/document.h>
#include <libany/rcpp/service.h>

namespace libany {
	namespace usersrvcli {
		class UserQuery : public ::libany::rcpp::Client {
			private:
				char _brand[128];
				char _base[128];
				int _st;
				::libany::rcpp::RcppDocument _sax;
			public:
				void list();
				bool getnext(char*, int);

				UserQuery(::libany::rcpp::RcppStream& r, const char*, const char*);
		};

		class User : protected ::libany::rcpp::Client {
			private:
				char _name[128];
				char _brand[128];
				char _service[128];
			public:
				User(::libany::rcpp::RcppStream&, 
						const char*, const char*, const char*);

				void create();
				bool check();
		};
	}
}

#endif
