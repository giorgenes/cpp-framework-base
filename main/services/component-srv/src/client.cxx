#include <libany/componentsrv/component.h>
#include <libany/rcpp/session.h>
#include <libany/stfactory/uristream.h>
#include <stdio.h>
#include "transaction.h"
#include "atomic.h"
#include "client.h"

namespace impl = ::componentsrv_impl;

impl::Client::Client(impl::DB* db)
	: _db(db)
{
}

::libany::rcpp::Transaction* impl::Client::new_transaction()
{
	return new Transaction(_db);
}

::libany::rcpp::AtomicTransaction* impl::Client::new_atomic_transaction()
{
	return new AtomicTransaction(_db);
}

impl::Client::~Client()
{
}


