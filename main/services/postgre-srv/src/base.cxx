#include "base.h"
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <libany/utils/path_utils.h>

using namespace postgresrv_impl;

BTransaction::BTransaction(PostgreDB* db)
	: _db(db)
{
}

BTransaction::~BTransaction()
{
}

void BTransaction::data(::libany::bxtp::IDocument& doc)
{
	::libany::dbsrv::BTransaction::data(doc);
}

