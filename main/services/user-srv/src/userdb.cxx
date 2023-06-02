#include "userdb.h"
#include <stdexcept>
#include <libany/utils/string_utils.h>
		
void UserDB::first()
{
	if(!_fp) {
		char buf[1024];

		sprintf(buf, "/tmp/usersrvdb.%s.%s.txt", brand, base);
		_fp = fopen(buf, "r");
		if(!_fp) {
			throw std::runtime_error("cant open usersrvdb");
		}
	}
}

bool UserDB::next(User& user)
{
	char buf[1024];

	if(!_fp) return false;
	
	if(fgets(buf, sizeof(buf), _fp)==NULL) {
		return false;
	}

	char* p = ::libany::utils::string_trimall(buf, 0);
	strcpy(user.name, p);

	return true;
}

UserDB::~UserDB()
{
	if(_fp) {
		fclose(_fp);
	}
}
