#include <libany/componentsrv/component.h>
#include <stdio.h>
#include "atomic.h"

using namespace componentsrv_impl;
namespace impl = componentsrv_impl;

AtomicTransaction::~AtomicTransaction()
{
}

void AtomicTransaction::begin()
{
	BTransaction::begin();
}

void AtomicTransaction::exec(::libany::bxtp::ODocument& doc)
{
	impl::BTransaction::exec(doc);
	
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

void AtomicTransaction::key(::libany::bxtp::IDocument& doc)
{
	impl::BTransaction::key(doc);
}

void AtomicTransaction::data(::libany::bxtp::IDocument& doc)
{
	impl::BTransaction::data(doc);
}

void AtomicTransaction::begin_exec(const char* service, const char* base)
{
	impl::BTransaction::begin_exec(service, base);
}

