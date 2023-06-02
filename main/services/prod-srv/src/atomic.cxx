#include <libany/prodsrv/product.h>
#include <stdio.h>
#include "atomic.h"

using namespace productsrv;

AtomicTransaction::~AtomicTransaction()
{
}

void AtomicTransaction::begin()
{
	BaseTransaction::begin();
}

void AtomicTransaction::exec(::libany::bxtp::ODocument& doc)
{
	try {
		BaseTransaction::exec(doc);
		fprintf(stderr, "oper = %d\n", _oper);
		if(_oper == OPER_CREATE) {
			ProductDB db(_brand, _base);
			Product product(db, _name);
			
			product.create();
		}
		else if(_oper == OPER_CHECK) {
			ProductDB db(_brand, _base);
			Product product(db, _name);
			bool exists = product.exists();
			doc.write_tag("exists", exists ? "1" : "0");
		}
		else if(_oper == OPER_LIST) {
			ProductDB db(_brand, _base);
			Product product(db);

			db.first();
			while(db.next(product)) {
				doc.write_tag("product", product.name);
			}
		}
		else if(_oper == OPER_CHECKOUT) {
			ProductDB db(_brand, _base);
			Product product(db, _name);

			product.send_files(_version, doc);
		}
		else if(_oper == OPER_COMMIT) {
			ProductDB db(_brand, _base);
			Product product(db, _name);

			product.commit(
					_version, 
					sizeof(_version),
					_tmppath,
					_tmppath_filelist);

			doc.write_tag("version", _version);
		}
		else if(_oper == OPER_LISTVER) {
			ProductDB db(_brand, _base);
			Product product(db, _name);

			product.send_versions(doc);
		}
	}
	catch(...) {
		throw;
	}

}

void AtomicTransaction::key(::libany::bxtp::IDocument& doc)
{
	BaseTransaction::key(doc);
}

void AtomicTransaction::data(::libany::bxtp::IDocument& doc)
{
	try {
		BaseTransaction::data(doc);
	}
	catch(...) {
		throw;
	}
}

void AtomicTransaction::begin_exec(const char* service, const char* base)
{
	BaseTransaction::begin_exec(service, base);
}

