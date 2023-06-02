#ifndef __libany__cgi__stdcgi_h
#define __libany__cgi__stdcgi_h

#include <libany/cgi/cgi.h>

namespace libany {
	namespace cgi {
		class StdCGI {
			protected:
				CGIApp& _app;
			public:
				StdCGI(CGIApp &app)
					: _app(app) {
				};

				void run(int argc, char**argv);
		};
	}
}

#endif
