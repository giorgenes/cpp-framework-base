#include "base.h"
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <libany/utils/path_utils.h>

using namespace postgresrv_impl;

Transaction::Transaction(PostgreDB* db)
	: _db(db)
{
}

Transaction::~Transaction()
{
}

void Transaction::data(::libany::bxtp::Document& doc)
{
	::libany::dbsrv::Transaction::data(doc);
}

