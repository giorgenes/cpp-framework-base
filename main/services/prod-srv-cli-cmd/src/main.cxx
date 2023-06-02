#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <libany/utils/path_utils.h>
#include <string.h>
#include <libany/prodsrvcli/product.h>
#include <libany/componentsrvcli/meta.h>
#include <libany/stfactory/uristream.h>
#include <stdexcept>
#include "opt.h"
#include "meta.h"
#include <vector>

namespace prodcli = ::libany::prodsrvcli;
namespace compcli = ::libany::componentsrvcli;


static void 
create(::libany::rcpp::RcppStream& stream, OptParser& opt, const char* name)
{
	try {
		prodcli::Product c(stream, 
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
	prodcli::Product c(stream, 
			name, opt.brand, opt.service);

	std::vector< prodcli::ComponentInfo> components;
	c.checkout(opt.version, &components);

	MetaDir md(name);
	md.create();
	md.save_file_list(components);
	md.write_version(opt.version);
}

static void 
commit(::libany::rcpp::RcppStream& stream, OptParser& opt, const char* name)
{
	try {
		std::vector< prodcli::ComponentInfo> files;
		MetaDir md(name);
		md.load_file_list(&files);
		char version[1024];
		md.read_version(version, sizeof(version));
		prodcli::Product c(stream, 
				name, opt.brand, opt.service);

		c.commit(version, sizeof(version), files);
		printf("new version: %s\n", version);
	}
	catch(...) {
		throw;
	}
}

static void 
list(::libany::rcpp::RcppStream& stream, OptParser& opt)
{
	prodcli::ProductQuery q(
			stream, 
			opt.brand, opt.service);

	char name[1024];
	q.list();
	while(q.getnext(name, sizeof(name))) {
		fprintf(stdout, "%s\n", name);
	}
}

static void 
listrev(::libany::rcpp::RcppStream& stream, OptParser& opt)
{
	prodcli::ProductQuery q(
			stream, 
			opt.name, opt.brand, opt.service);

	char number[1024];
	char childs[2048];
	q.get_versions();
	while(q.next_version(number, sizeof(number), childs, sizeof(childs))) {
		fprintf(stdout, "%s %s\n", number, childs);
	}
}

static void 
add(OptParser& opt)
{
	compcli::MetaDir compmd(0);
	MetaDir md(0);
}




int main(int argc, char** argv)
{
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
	}
	catch(std::exception &e) {
		fprintf(stderr, "exception found: %s\n", e.what());
	}
	return 0;
}

