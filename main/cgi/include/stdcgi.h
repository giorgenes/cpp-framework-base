#ifndef __libany__cgi__stdcgi_h
#define __libany__cgi__stdcgi_h

#include <libany/cgi/cgi.h>

namespace libany {
	namespace cgi {
		class StdCGI {
			protected:
				CGI& _cgi;
			public:
				StdCGI(CGI &cgi)
					: _cgi(cgi) {
				};

				void run(int argc, char**argv);
		};
	}
}

#endif
