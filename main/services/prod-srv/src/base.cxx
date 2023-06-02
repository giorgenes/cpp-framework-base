#include "base.h"
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <libany/utils/path_utils.h>

using namespace productsrv;

void BaseTransaction::begin()
{
}

BaseTransaction::~BaseTransaction()
{
}

void BaseTransaction::exec(::libany::bxtp::ODocument&)
{
}

void BaseTransaction::key(::libany::bxtp::IDocument& doc)
{
	const char *opts[] = {
		"/key/name",
		"/key/brand",
		"/key/service",
		0
	};

	int len;
	int idx;
	while(doc.match(opts, &idx)) {
		switch(idx) {
			case 0:
				len = doc.read(_name, sizeof(_name)-1);
				_name[len] = 0;
				break;
			case 1:
				len = doc.read(_brand, sizeof(_brand)-1);
				_brand[len] = 0;
				break;
			case 2:
				len = doc.read(_base, sizeof(_base)-1);
				_base[len] = 0;
				break;
		}
	}
}

void 
BaseTransaction::get_file(::libany::bxtp::IDocument& doc, FILE* fp)
{
	try {
		int len;
		
		doc.match("/component/name", true);
		char id[1024];
		len = doc.read(id, sizeof(id)-1);
		id[len] = 0;
		
		doc.match("/component/revision", true);
		char path[1024];
		len = doc.read(path, sizeof(path)-1);
		path[len] = 0;

		fprintf(fp, "%s;%s\n", id, path);
	}
	catch(...) {
		throw;
	}
}

void BaseTransaction::get_files(::libany::bxtp::IDocument& doc)
{
	try {
		strcpy(_tmppath_filelist, "/tmp/productsrv_XXXXXX");
		int fd;
		if((fd=mkstemp(_tmppath_filelist))==-1) {
			throw std::runtime_error("could not create tmp file");
		}
		close(fd);

		FILE *fp = fopen(_tmppath_filelist, "w");
		if(!fp) {
			throw std::runtime_error("could not create tmp file");
		}

		while(doc.match("/data/commit/components/component")) {
			::libany::bxtp::IDocument filedoc(doc);

			get_file(filedoc, fp);
		}

		/* FIXME: o arquivo fica aberto
		 * se der uma excessao no trecho acima */
		fclose(fp);
	}
	catch(...) {
		throw;
	}
}

void BaseTransaction::get_version(
		const char* path, 
		::libany::bxtp::IDocument& doc)
{
	if(!doc.match(path)) {
		throw std::runtime_error("couldn't get version");
	}
	int len = doc.read(_version, sizeof(_version)-1);
	if(len <= 0) {
		throw std::runtime_error("empty version");
	}
	_version[len] = 0;

}

void BaseTransaction::data(::libany::bxtp::IDocument& doc)
{
	const char *opts[] = {
		"/data/create",
		"/data/list",
		"/data/check",
		"/data/checkout",
		"/data/commit",
		"/data/listver",
		0
	};

	int idx;

	try {
		if(doc.match(opts, &idx)) {
			switch(idx) {
				case 0:
					_oper = OPER_CREATE;
					break;
				case 1:
					_oper = OPER_LIST;
					break;
				case 2:
					_oper = OPER_CHECK;
					break;
				case 3:
					get_version("/data/checkout/version", doc);
					_oper = OPER_CHECKOUT;
					break;
				case 4:
					get_version("/data/commit/version", doc);
					get_files(doc);
					_oper = OPER_COMMIT;
					break;
				case 5:
					_oper = OPER_LISTVER;
					break;
			}
		}
		else {
			_oper = OPER_NONE;
		}
	}
	catch(...) {
		throw;
	}
}

void BaseTransaction::begin_exec(const char* service, const char*)
{
	*_name    = 0;
	*_brand   = 0;
	*_base    = 0;
	*_tmppath = 0;
}

