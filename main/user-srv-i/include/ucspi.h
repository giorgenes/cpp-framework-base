#ifndef __libany__stdsvri__ucspi_h
#define __libany__stdsvri__ucspi_h

#include <libany/ios/ios.h>

namespace libany {
	namespace stdsrvi {		
		class Ucspi {
			private:
				::libany::ios::Service& _service;
			public:
				Ucspi(::libany::ios::Service& s)
					: _service(s) {};

				void run();
		};
	}
}

#endif
