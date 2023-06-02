#include <libany/usersrv/user.h>
#include <stdio.h>
#include "atomic.h"

using namespace usersrv_impl;

AtomicTransaction::~AtomicTransaction()
{
}

void AtomicTransaction::begin()
{
	BaseTransaction::begin();
}

void AtomicTransaction::exec(::libany::bxtp::ODocument& st)
{
	try {
		BaseTransaction::exec(st);
		//fprintf(stderr, "oper = %d, user = %s, brand = %s, base = %s\n",
		//	_oper, _user.name, _user.brand, _user.base);
		if(_oper == OPER_CREATE) {
			UserDB db(_brand, _base);
			User user(db, _name);
			
			user.create();
		}
		else if(_oper == OPER_CHECK) {
			UserDB db(_brand, _base);
			User user(db, _name);
			bool exists = user.exists();
			st.write_tag("exists", exists ? "1" : "0");
		}
		else if(_oper == OPER_LIST) {
			UserDB db(_brand, _base);
			User user(db);

			db.first();
			while(db.next(user)) {
				st.write_tag("user", user.name);
			}
		}
	}
	catch(...) {
		throw;
	}
}

void AtomicTransaction::begin_exec(const char* service, const char* base)
{
	BaseTransaction::begin_exec(service, base);
}

void AtomicTransaction::key(::libany::bxtp::IDocument& doc)
{
	BaseTransaction::key(doc);
}

void AtomicTransaction::data(::libany::bxtp::IDocument& doc)
{
	BaseTransaction::data(doc);
}
