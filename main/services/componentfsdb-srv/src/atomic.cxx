#include "srv.h"
#include <stdio.h>
#include "atomic.h"
#include <libany/utils/string_utils.h>
#include <stdexcept>

using namespace ::libany::componentfsdbsrv;
namespace impl = ::libany::componentfsdbsrv;

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
	
	char* params[10];
	char** p = &params[1];
	int n = libany::utils::string_split(__query, " \t", params, 10);

	if(check_param(*params, "get_component_names", n, 0)) {
		Component component(_db, "");
		component.write_components(doc);
	}
	else {
		throw std::runtime_error(
				std::string("unrecognized command ") + *params);
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

