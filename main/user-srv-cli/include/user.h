#ifndef __libany__usersrvcli__user_h
#define __libany__usersrvcli__user_h

#include <libany/ios/ios.h>

namespace libany {
	namespace usersrvcli {
		class User : protected ::libany::ios::Client {
			public:
				User(::libany::ios::Router&, 
						const char*, const char*, const char*);

				void create();
		};
	}
}

#endif
