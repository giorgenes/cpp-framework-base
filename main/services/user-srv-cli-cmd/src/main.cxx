#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <string.h>
#include <libany/usrsrvcli/user.h>
#include <libany/stdstream/filestream.h>
#include <libany/iniparser/parser.h>
#include <libany/stfactory/uristream.h>
#include <stdexcept>
#include "opt.h"

static void
exec_command(Opt& opt, 
		::libany::rcpp::RcppStream& stream,
		char* const * args, 
		int nargs)
{
	try {
		if(strcmp(args[0], "create")==0) {
			if(nargs != 4) {
				fprintf(stderr, "wrong number of args\n");
				return;
			}

			::libany::usersrvcli::User user(
					stream, args[1], args[2], args[3]);

			user.create();
		}
		else if(strcmp(args[0], "check")==0) {
			if(nargs != 4) {
				fprintf(stderr, "wrong number of args\n");
				return;
			}

			::libany::usersrvcli::User user(
					stream, args[1], args[2], args[3]);

			fprintf(stdout, "%d\n", user.check() ? 1 : 0);
		}
		else if(strcmp(args[0], "list")==0) {
			if(nargs != 3) {
				fprintf(stderr, "wrong number of args\n");
				return;
			}

			::libany::usersrvcli::UserQuery query(
					stream, args[1], args[2]);

			query.list();
			char user[1024];
			while(query.getnext(user, sizeof(user))) {
				fprintf(stdout, "uhuser: %s\n", user);
			}
		}

		else {
			fprintf(stderr, "command not understood\n");
		}
	}
	catch(...) {
		throw;
	}
}

void interactive(Opt& opt, ::libany::rcpp::RcppStream& stream)
{
	char buf[1024];
	char* p;
	char* args[10];
	int nargs;

	while(fgets(buf, sizeof(buf), stdin) != NULL) {
		p = ::libany::utils::string_trimall(buf, NULL);
		if(!*p || *p=='#') continue;

		nargs = ::libany::utils::string_split(p, " \t", args, 10);

		try {
			exec_command(opt, stream, args, nargs);
		}
		catch(std::exception &e) {
			fprintf(stderr, "exception found: %s\n", e.what());
		}

	}
}

int main(int argc, char** argv)
{
	try {
		Opt opt;

		opt.parse();

		::libany::stfactory::URIStream stream(opt.base);
		::libany::rcpp::RcppStream rcppstream(stream);
		interactive(opt, rcppstream);
	}
	catch(std::exception &e) {
		fprintf(stderr, "exception found: %s\n", e.what());
	}
	return 0;
}

