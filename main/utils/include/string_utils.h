#ifndef __libany__utils__string_utils_h
#define __libany__utils__string_utils_h

namespace libany {
	namespace utils {
		char* string_trimall(char*, int*);
		int string_split(char*, const char*, char**, int);
	}
}

#endif
