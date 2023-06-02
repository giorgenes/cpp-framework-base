#include "srv.h"
#include <stdio.h>
#include "transaction.h"
#include "atomic.h"
#include "client.h"

namespace impl = postgresrv_impl;
using namespace ::libany::postgresrv;

::libany::rcpp::Transaction* impl::Client::new_transaction()
{
	return new impl::Transaction(_db);
}

::libany::rcpp::AtomicTransaction* impl::Client::new_atomic_transaction()
{
	return new impl::AtomicTransaction(_db);
}

impl::Client::~Client()
{
}

