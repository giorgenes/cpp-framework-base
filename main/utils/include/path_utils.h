#ifndef __libany__utils__path_utils_h
#define __libany__utils__path_utils_h

namespace libany {
	namespace utils {
		void path_create(const char*);
		void getrealpath(char* path, int pathsize);
	}
}

#endif
