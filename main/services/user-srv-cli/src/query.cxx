#include <libany/usrsrvcli/user.h>
#include <stdexcept>
#include <libany/stfactory/uristream.h>
#include <libany/bxtp/document.h>
#include <memory>

using namespace ::libany::usersrvcli;
				
UserQuery::UserQuery(::libany::rcpp::RcppStream& r, 
		const char* brand, 
		const char* base)
	: ::libany::rcpp::Client(r), _sax(r)
{
	strcpy(_brand, brand);
	strcpy(_base, base);
}

void UserQuery::list()
{
	try {
		_sax.request_begin_nquery("user");
		_sax.request_key("brand", _brand);
		_sax.request_key("base", _base);
		_sax.request_data("list");
		_sax.request_end();
		_st = 1;
	}
	catch(...) {
		throw;
	}
}

bool UserQuery::getnext(char* user, int size)
{
	if(!_sax.match("/response/user")) {
		return false;
	}
	
	int len = _sax.read(user, size-1);
	if(len <= 0) {
		return false;
	}
	user[len] = 0;
	
	return true;
}

