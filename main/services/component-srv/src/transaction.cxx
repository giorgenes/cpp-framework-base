#include "transaction.h"
#include <stdio.h>

using namespace componentsrv_impl;
namespace impl = componentsrv_impl;

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

void Transaction::begin()
{
}

Transaction::~Transaction()
{
}

void Transaction::exec(::libany::bxtp::ODocument& doc)
{
	impl::BTransaction::exec(doc);
	
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

void Transaction::key(::libany::bxtp::IDocument& doc)
{
	impl::BTransaction::key(doc);
}

void Transaction::data(::libany::bxtp::IDocument& doc)
{
	impl::BTransaction::data(doc);
}

void Transaction::begin_exec(const char* service, const char* base)
{
	impl::BTransaction::begin_exec(service, base);
}

