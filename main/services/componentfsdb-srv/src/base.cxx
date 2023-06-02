#include "base.h"
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <libany/utils/path_utils.h>
#include <libany/stdstream/bufstream.h>

using namespace ::libany::componentfsdbsrv;

Transaction::~Transaction()
{
}

void Transaction::data(::libany::bxtp::Document& doc)
{
	::libany::dbsrv::Transaction::data(doc);
}

bool
Transaction::check_param(
		const char* name, const char* check, int n, int desired)
{
	if(strcmp(name, check)) {
		return false;
	}
	
	/* ignore the first params, wich is the
	 * function name */
	n--;
	
	if(desired >= 0) {
		if(desired == n) {
			return true;
		}
	}
	else {
		desired *= -1;
		if(n >= desired) {
			return true;
		}
	}

	throw std::runtime_error(
			std::string("invalid number of args. "));
}


