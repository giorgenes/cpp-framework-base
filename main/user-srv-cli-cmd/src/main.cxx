#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <string.h>
#include <libany/usrsrvcli/user.h>
#include <libany/stdstream/filestream.h>
#include <libany/stdstream/xmlstream.h>
#include <libany/srvrouter/router.h>
#include <stdexcept>

static void
exec_command(::libany::srvrouter::Router& p_router, 
		char* const * args, 
		int nargs)
{
	if(strcmp(args[0], "create")==0) {
		const char* username;
		const char* brand;
		const char* base;

		::libany::usersrvcli::User user(p_router, username, brand, base);

		user.create();
	}
	else {
		fprintf(stderr, "command not understood\n");
	}
}

class XmlConfReader : public ::libany::stdstream::OXmlStream {
	private:
		::libany::srvrouter::Router& _router;
	public:
		void tag_process_callback(const char*, const char*);
		XmlConfReader(::libany::srvrouter::Router& r)
			: _router(r) {};
};

void XmlConfReader::tag_process_callback(const char* tag, const char* value)
{
	printf("%s\n", tag);
	_router.configure(tag, value);
}

static void read_conf(::libany::srvrouter::Router& p_router)
{
	try {
		::libany::stdstream::IFileStream file("./config.xml");
		XmlConfReader reader(p_router);

		reader.copyfrom(&file);
	}
	catch(...) {
		throw;
	}
}

int main(int argc, char** argv)
{
	char buf[1024];
	char* p;
	char* args[10];
	int nargs;
	::libany::srvrouter::Router router;

	try {
		read_conf(router);

		while(fgets(buf, sizeof(buf), stdin) != NULL) {
			p = ::libany::utils::string_trimall(buf, NULL);
			if(!*p || *p=='#') continue;
	
			nargs = ::libany::utils::string_split(p, " \t", args, 10);

			exec_command(router, args, nargs);
		}
	}
	catch(std::exception &e) {
		fprintf(stderr, "exception found: %s\n", e.what());
	}
	return 0;
}

