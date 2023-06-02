#ifndef __user_h
#define __user_h

#include "userdb.h"

class UserDB;
class User {
	public:
		UserDB& db;
		char name[128];

		bool exists();
		void create();

		User(UserDB& d, const char* n)
			: db(d) 
		{
			strcpy(name, n);	
		};
		
		User(UserDB& d)
			: db(d) 
		{
			*name = 0;
		};

};

#endif
