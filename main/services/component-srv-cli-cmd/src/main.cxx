#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <libany/utils/path_utils.h>
#include <string.h>
#include <libany/componentsrvcli/component.h>
#include <libany/componentsrvcli/meta.h>
#include <libany/stfactory/uristream.h>
#include <stdexcept>
#include "opt.h"
#include <vector>
#include <signal.h>

namespace cli = ::libany::componentsrvcli;

static void 
create(::libany::rcpp::RcppStream& stream, OptParser& opt, const char* name)
{
	try {
		cli::Component c(stream, 
				name, opt.brand, opt.service);

		c.create();
	}
	catch(...) {
		throw;
	}
}

static void 
checkout(::libany::rcpp::RcppStream& stream, OptParser& opt, const char* name)
{
	try {
		cli::Component c(stream, 
				name, opt.brand, opt.service);

		std::vector< cli::FileInfo> files;
		char buf[1024];
		sprintf(buf, "./%s", name);
		c.checkout(buf, opt.revision, &files);

		cli::MetaDir md(buf);
		md.create();
		md.save_file_list(files);
		md.write_revision(opt.revision);
		md.write_name(name);
	}
	catch(...) {
		throw;
	}
}

static void 
commit(::libany::rcpp::RcppStream& stream, OptParser& opt, const char* path)
{
	try {
		std::vector< cli::FileInfo> files;
		cli::MetaDir md(path);
		md.load_file_list(&files);
		char revision[1024];
		md.read_revision(revision, sizeof(revision));
		char name[1024];
		md.read_name(name);
		cli::Component c(stream, 
				name, opt.brand, opt.service);

		c.commit(md.path(), revision, sizeof(revision), files);
		printf("new revision: %s\n", revision);
		md.write_revision(revision);
	}
	catch(...) {
		throw;
	}
}

static void 
list(::libany::rcpp::RcppStream& stream, OptParser& opt)
{
	try {
		cli::ComponentQuery q(
				stream, 
				opt.brand, opt.service);

		char name[1024];
		q.list();
		while(q.getnext(name, sizeof(name))) {
			fprintf(stdout, "%s\n", name);
		}
		
	}
	catch(...) {
		throw;
	}
}

static void 
listrev(::libany::rcpp::RcppStream& stream, OptParser& opt)
{
	cli::ComponentQuery q(
			stream, 
			opt.name, opt.brand, opt.service);

	char number[1024];
	char childs[2048];
	q.get_revisions();
	while(q.next_revision(number, sizeof(number), childs, sizeof(childs))) {
		fprintf(stdout, "%s %s\n", number, childs);
	}
}

static void 
add(OptParser& opt)
{
	cli::MetaDir md(0);

	for(unsigned int i = 0; i<opt.files.size(); i++) {
		md.add(opt.files[i]);
	}
}

int main(int argc, char** argv)
{
	signal(SIGPIPE, SIG_IGN);
	try {
		OptParser opt;

		opt.parse(argc, argv);
		
		::libany::stfactory::URIStream stream(opt.base_uri);
		::libany::rcpp::RcppStream rcppstream(stream);

		if(COMMAND_CREATE == opt.command) {
			create(rcppstream, opt, opt.name);
		}
		else if(COMMAND_CHECKOUT == opt.command) {
			checkout(rcppstream, opt, opt.name);
		}
		else if(COMMAND_LIST == opt.command) {
			list(rcppstream, opt);
		}
		else if(COMMAND_COMMIT == opt.command) {
			commit(rcppstream, opt, opt.name);
		}
		else if(COMMAND_LISTREV == opt.command) {
			listrev(rcppstream, opt);
		}
		else if(COMMAND_ADD == opt.command) {
			add(opt);
		}

		rcppstream.commit();
	}
	catch(std::exception &e) {
		fprintf(stderr, "exception found: %s\n", e.what());
	}
	return 0;
}

