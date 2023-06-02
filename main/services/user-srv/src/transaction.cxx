#include "transaction.h"
#include <stdio.h>

using namespace usersrv_impl;

void Transaction::commit()
{
	fprintf(stderr, "commit\n");
}

void Transaction::rollback()
{
	fprintf(stderr, "rollback\n");
}

void Transaction::begin()
{
}

Transaction::~Transaction()
{
}

void Transaction::begin_exec(const char*, const char*)
{
}

void Transaction::exec(::libany::bxtp::ODocument& doc)
{
	BaseTransaction::exec(doc);
}


void Transaction::key(::libany::bxtp::IDocument& doc)
{
	BaseTransaction::key(doc);
}

void Transaction::data(::libany::bxtp::IDocument& doc)
{
	BaseTransaction::data(doc);
}


