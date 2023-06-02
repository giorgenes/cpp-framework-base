#ifndef __libany__except__except__h
#define __libany__except__except__h

#include <stdexcept>

namespace libany {
	namespace except {
		class Errno : public std::exception {
			private:
				int __errno;
				char __msg[1024];
			public:
				Errno(int e);
				Errno();

				virtual const char* what() const throw();
		};
	}
}

#endif
