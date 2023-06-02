#include "srv.h"
#include "base.h"
#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <stdexcept>

using namespace ::libany::componentfsdbsrv;
namespace impl = ::libany::componentfsdbsrv;

void Transaction::nexec(::libany::bxtp::Document& doc)
{
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

