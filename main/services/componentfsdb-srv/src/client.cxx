#include "srv.h"
#include <libany/rcpp/session.h>
#include <libany/stfactory/uristream.h>
#include <stdio.h>
#include "transaction.h"
#include "client.h"

namespace impl = ::libany::componentfsdbsrv;

impl::Client::Client(impl::DB* db)
	: _db(db)
{
}

::libany::rcpp::Transaction* impl::Client::new_transaction()
{
	return new Transaction(_db);
}

impl::Client::~Client()
{
}


