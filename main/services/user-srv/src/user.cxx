#include "user.h"
#include <stdexcept>
#include <stdio.h>
#include <libany/utils/string_utils.h>

void User::create()
{
	try {
		if(exists()) {
			throw std::runtime_error("user exists");
		}
		FILE* fp;
		char buf[1024];

		sprintf(buf, "/tmp/usersrvdb.%s.%s.txt", db.brand, db.base);
		fp = fopen(buf, "a");
		if(!fp) {
			throw std::runtime_error("cant open usersrvdb");
		}
		fprintf(fp, "%s\n", name);
		fclose(fp);
	}
	catch(...) {
		throw;
	}
}

bool User::exists()
{
	FILE* fp;
	char buf[1024];

	sprintf(buf, "/tmp/usersrvdb.%s.%s.txt", db.brand, db.base);
	fp = fopen(buf, "r");
	if(!fp) {
		throw std::runtime_error("cant open usersrvdb");
	}

	char* p;
	bool e = false;
	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		p = ::libany::utils::string_trimall(buf, 0);
		if(strcmp(p, name)==0) {
			e = true;
			break;
		}
	}

	fclose(fp);
	
	return e;
}

