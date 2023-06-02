#include "transaction.h"
#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <stdexcept>

using namespace ::libany::componentfsdbsrv;
namespace impl = ::libany::componentfsdbsrv;

namespace lc {
	using namespace ::libany::utils;
}

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

	char* params[10];
	char** p = &params[1];
	int n = libany::utils::string_split(__query, " \t", params, 10);

	if(check_param(*params, "create", n, 1)) {
		Component component(_db, p[0]);
		component.create();
	}
	else if(check_param(*params, "add_revision", n, 2)) {
		Component component(_db, p[0]);
		component.add_revision(p[1]);
	}
	else if(check_param(*params, "set_next_revision", n, 3)) {
		Component component(_db, p[0]);
		component.write_next_revision(p[1], atoi(p[2]));
	}
	else {
		throw std::runtime_error(
				std::string("unrecognized command ") + *params);
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

