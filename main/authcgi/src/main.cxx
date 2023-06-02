#include <libany/cgi/cgi.h>
#include <libany/cgi/stdcgi.h>
#include <libany/stdstream/urldecode.h>
#include <stdio.h>
#include <string.h>

namespace cgi = ::libany::cgi;
namespace stream {
	using namespace ::libany::stdstream;
}

class AuthCGI : public cgi::CGIApp {
	private:
		bool _authok;
	public:
		void write_header(const cgi::CGIEnv&, cgi::Header&);
		void write_body(const cgi::CGIEnv&);

		AuthCGI();
};

AuthCGI::AuthCGI()
	: _authok(false)
{
}

void AuthCGI::write_header(
		const cgi::CGIEnv& env, cgi::Header& header)
{
	header.write_content_type("text/xml");

	stream::IUrlDecodeStream in(env.io);

	char user[1024]="";
	char pass[1024]="";
	
	while(in.get_next_pair()) {
		if(strcmp(in.key(), "user")==0) {
			in.read(user, sizeof(user));
		}
		else if(strcmp(in.key(), "pass")==0) {
			in.read(pass, sizeof(pass));
		}
	}

	if(!*pass || !*user) {
		return;
	}

	_authok = true;
	header.write_cookie(
			"authuser", 
			user, 
			"saga.vshouse", 
			"/", 
			time(NULL)+1000, 
			false);

	header.write_cookie(
			"authpass", 
			pass, 
			"saga.vshouse", 
			"/", 
			time(NULL)+1000, 
			false);
}

void AuthCGI::write_body(const cgi::CGIEnv& env)
{
	if(_authok) {
		env.io.write("<auth>0</auth>", 14);
	}
	else {
		env.io.write("<auth>1</auth>", 14);
	}
}

int main(int argc, char** argv)
{
	AuthCGI c;
	cgi::StdCGI cgi(c);

	cgi.run(argc, argv);

	return 0;
}

