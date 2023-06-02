#include <libany/usrsrvcli/user.h>
#include <stdexcept>
#include <libany/stfactory/uristream.h>
#include <memory>

using namespace ::libany::usersrvcli;

User::User(::libany::rcpp::RcppStream& r, 
		const char* name, 
		const char* brand, 
		const char* service)
	: ::libany::rcpp::Client(r)
{
	strcpy(_name, name);
	strcpy(_brand, brand);
	strcpy(_service, service);
}

void User::create()
{
	try {
		::libany::rcpp::RcppDocument cmd(_rcppstream);

		cmd.request_begin_nquery("user");
		cmd.request_key("username", _name);
		cmd.request_key("brand", _brand);
		cmd.request_key("base", _service);
		cmd.request_data("create");
		cmd.request_end();

		cmd.match_end();
	}
	catch(...) {
		throw;
	}
}

bool User::check()
{
	try {
		
		::libany::rcpp::RcppDocument cmd(_rcppstream);
		
		cmd.request_begin_nquery("user");
		cmd.request_key("username", _name);
		cmd.request_key("brand", _brand);
		cmd.request_key("base", _service);
		cmd.request_data("check");
		cmd.request_end();

		if(!cmd.match("/response/exists")) {
			throw std::runtime_error("wrong return");
		}

		char buf[32];
		int len = cmd.read(buf, sizeof(buf)-1);

		if(len <= 0) {
			throw std::runtime_error("wrong return");
		}
		buf[len] = 0;

		bool exists = atoi(buf);
		return exists;
	}
	catch(...) {
		throw;
	}

	return false;
}

