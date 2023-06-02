#include "transaction.h"
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <libany/utils/path_utils.h>

using namespace ::libany::dbsrv;

Transaction::Transaction()
	: __qlen(0), __query(0)
{
}

Transaction::~Transaction()
{
}

void Transaction::data(::libany::bxtp::Document& doc)
{
	if(doc.match("/data/query")) {
		int len;
		char buf[1024];
		if(__query) {
			__qlen = 0;
			free(__query);
			__query = 0;
		}
		while((len=doc.read(buf, sizeof(buf)))>0) {
			__query = static_cast<char*>(realloc(__query, __qlen + len + 1));
			strncpy(__query + __qlen, buf, len);
			__qlen += len;
		}
		if(__qlen == 0) {
			throw std::runtime_error("empty query");
		}
		__query[__qlen] = 0;
	}
}

