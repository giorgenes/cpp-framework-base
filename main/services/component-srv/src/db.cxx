#include "db.h"
#include <stdexcept>
#include <libany/iniparser/parser.h>
#include <libany/stdstream/filestream.h>
#include <libany/except/except.h>

namespace impl = componentsrv_impl;

void impl::DB::commit()
{
	_rcpp->commit();
}

void impl::DB::read_config(const char* path)
{
	::libany::stdstream::IFileStream file(path);
	::libany::iniparser::Parser parser(file);

	char s[1024];
	char k[1024];
	char v[1024];
	while(parser.next_conf(s, sizeof(s), k, sizeof(k), v, sizeof(v))) {
		if(strcmp(s, "componentsrv")==0) {
			if(strcmp(k, "base_uri")==0) {
				strcpy(__base_uri, v);
			}
			else if(strcmp(k, "strfile")==0) {
				strcpy(__strfile_path, v);
			}

		}
	}
}

impl::DB::DB()
{
	try {
		const char* path = getenv("VSHCONFIG");
		if(path) {
			read_config(path);
		}
		else {
			read_config("config.ini");
		}

		_stream = new ::libany::stfactory::URIStream(__base_uri);
		_rcpp = new ::libany::rcpp::RcppStream(*_stream);

		_strfile.load(__strfile_path);
	}
	catch(std::exception &e) {
		fprintf(stderr, "%s\n", e.what());
		throw;
	}
	catch(...) {
		throw;
	}
}

impl::DB::~DB()
{
}

void impl::DB::begin_transaction()
{
}

