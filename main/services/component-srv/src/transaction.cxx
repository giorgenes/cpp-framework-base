#include "transaction.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <libany/utils/path_utils.h>


using namespace componentsrv_impl;
namespace impl = componentsrv_impl;
namespace bxtp = ::libany::bxtp;

void Transaction::commit()
{
	_db->commit();
	fprintf(stderr, "commit\n");
}

void Transaction::rollback()
{
	//_db->rollback();
	fprintf(stderr, "rollback\n");
}

void Transaction::exec(bxtp::Document& doc)
{
	impl::Transaction::exec(doc);
	
	fprintf(stderr, "oper = %d\n", _oper);
	if(_oper == OPER_CREATE) {
		ComponentDB db(_db, _brand, _base);
		Component component(db, _name);

		component.create();
	}
	else if(_oper == OPER_CHECK) {
		ComponentDB db(_db, _brand, _base);
		Component component(db, _name);
		bool exists = component.exists();
		doc.write_tag("exists", exists ? "1" : "0");
	}
	else if(_oper == OPER_CHECKOUT) {
		ComponentDB db(_db, _brand, _base);
		Component component(db, _name);

		component.send_files(_revision, doc);
	}
	else if(_oper == OPER_COMMIT) {
		ComponentDB db(_db, _brand, _base);
		Component component(db, _name);

		component.commit(
				_revision, 
				sizeof(_revision),
				_tmppath,
				_tmppath_filelist);

		doc.write_tag("revision", _revision);
	}
}

void Transaction::nexec(bxtp::Document& doc)
{
	impl::Transaction::exec(doc);
	
	if(_oper == OPER_LIST) {
		ComponentDB db(_db, _brand, _base);

		db.write_components(doc);
	}
	else if(_oper == OPER_LISTVER) {
		ComponentDB db(_db, _brand, _base);
		Component component(db, _name);

		component.send_revisions(doc);
	}
}

void Transaction::begin()
{
}

Transaction::~Transaction()
{
}

void Transaction::key(bxtp::Document& doc)
{
	const char *opts[] = {
		"/key/name",
		"/key/brand",
		"/key/base",
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

static void 
write2file(const char* path, ::libany::stream::Stream& in)
{
	try {
		char* p;
		if((p = strrchr(path, '/'))) {
			char base[1024];
			strncpy(base, path, p - path);
			base[p - path] = 0;
			::libany::utils::path_create(base);
		}
		
		FILE* fp = fopen(path, "w");
		if(!fp) {
			throw std::runtime_error("could not create file");
		}

		char buf[1024];
		int len;
		while((len = in.read(buf, sizeof(buf))) > 0) {
			fwrite(buf, 1, len, fp);
		}
		
		fclose(fp);
	}
	catch(...) {
		throw;
	}
}

void 
Transaction::get_file(bxtp::Document& doc, FILE* fp)
{
	try {
		int len;
		
		doc.match("/file/id", true);
		char id[1024];
		len = doc.read(id, sizeof(id)-1);
		id[len] = 0;
		
		doc.match("/file/path", true);
		char path[1024];
		len = doc.read(path, sizeof(path)-1);
		path[len] = 0;

		doc.match("/file/content", true);

		char buf[1024];
		sprintf(buf, "%s/%s", _tmppath, path);
		write2file(buf, doc);

		fprintf(fp, "%s;%s\n", id, path);
	}
	catch(...) {
		throw;
	}
}

void Transaction::get_files(bxtp::Document& doc)
{
	try {
		strcpy(_tmppath, "/tmp/componentsrv_XXXXXX");
		if(mkdtemp(_tmppath)==NULL) {
			*_tmppath = 0;
			throw std::runtime_error("could not create tmp path");
		}
		strcpy(_tmppath_filelist, "/tmp/componentsrv_XXXXXX");
		int fd;
		if((fd=mkstemp(_tmppath_filelist))==-1) {
			throw std::runtime_error("could not create tmp file");
		}
		close(fd);

		FILE *fp = fopen(_tmppath_filelist, "w");
		if(!fp) {
			throw std::runtime_error("could not create tmp file");
		}

		while(doc.match("/data/commit/files/file")) {
			bxtp::Document filedoc(doc);

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

void Transaction::get_revision(
		const char* path, 
		bxtp::Document& doc)
{
	if(!doc.match(path)) {
		throw std::runtime_error("couldn't get revision");
	}
	int len = doc.read(_revision, sizeof(_revision)-1);
	if(len <= 0) {
		throw std::runtime_error("empty revision");
	}
	_revision[len] = 0;

}

void Transaction::data(bxtp::Document& doc)
{
	const char *opts[] = {
		"/data/create",
		"/data/list",
		"/data/check",
		"/data/checkout",
		"/data/commit",
		"/data/listrev",
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
					get_revision("/data/checkout/revision", doc);
					_oper = OPER_CHECKOUT;
					break;
				case 4:
					get_revision("/data/commit/revision", doc);
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

void Transaction::begin_exec(const char* service, const char*)
{
	*_name    = 0;
	*_brand   = 0;
	*_base    = 0;
	*_tmppath = 0;
}

