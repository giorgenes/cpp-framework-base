#ifndef __userdb_h
#define __userdb_h

#include <string.h>
#include "user.h"
#include <stdio.h>

class User;
class UserDB {
	private:
		FILE* _fp;
	public:
		char base[128];
		char brand[128];

		UserDB(const char* _brand, const char* _base)
			: _fp(0) {
			strcpy(brand, _brand);
			strcpy(base, _base);
		};

		void first();
		bool next(User&);

		~UserDB();
};

#endif
