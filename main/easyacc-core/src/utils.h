#ifndef __utils_h
#define __utils_h

#include <stdio.h>

class File {
	private:
		FILE* fp;
		bool owner;
	public:
		void open(const char*, const char*);
		void close();

		inline operator FILE* () {
			return fp;
		};

		File() : fp(NULL), owner(false) {
		};

		~File() {
			this->close();
		};
};

#endif
