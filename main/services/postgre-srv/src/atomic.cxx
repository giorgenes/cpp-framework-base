//#include <libany/postgresrv/postgre.h>
#include <stdio.h>
#include "atomic.h"

using namespace postgresrv_impl;

AtomicTransaction::~AtomicTransaction()
{
}

void AtomicTransaction::begin()
{
	BTransaction::begin();
}

void AtomicTransaction::exec(::libany::bxtp::ODocument& doc)
{
	BTransaction::exec(doc);
}

void AtomicTransaction::key(::libany::bxtp::IDocument& doc)
{
	BTransaction::key(doc);
}

void AtomicTransaction::data(::libany::bxtp::IDocument& doc)
{
	BTransaction::data(doc);
}

void AtomicTransaction::begin_exec(const char* service, const char* base)
{
	BTransaction::begin_exec(service, base);
}

