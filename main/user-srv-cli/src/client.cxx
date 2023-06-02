#include <libany/usrsrvcli/user.h>
#include <libany/rcpp/session.h>
#include <libany/stfactory/uristream.h>
#include <memory>

using namespace ::libany::usersrvcli;

User::User(::libany::ios::Router& r, const char*, const char*, const char*)
	: ::libany::ios::Client(r)
{
}

void User::create()
{
	try {
		const char* uri = _router.route("user", "");
		::libany::stfactory::URIStream stream(uri);
		::libany::rcpp::ClientSession session(&stream);

		session.handshake();

		::libany::rcpp::PostL1 cmd(&session);

		cmd.send_begin('Q');
		cmd.next_group(); {
			cmd.next_field();
			cmd.write("user", 4);
		}
		
		cmd.next_group(); {
			cmd.next_field();
			cmd.write("username", 8);
		
			cmd.next_field();
			cmd.write("brand", 5);
		
			cmd.next_field();
			cmd.write("base", 4);
		}

		cmd.next_group(); {
			cmd.next_field();
			cmd.write("create", 6);
		}

		cmd.send_end();
	}
	catch(...) {
		throw;
	}
}

