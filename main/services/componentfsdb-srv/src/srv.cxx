#include <libany/componentsrv/component.h>
#include <libany/rcpp/session.h>
#include <libany/stfactory/uristream.h>
#include <stdio.h>
#include "transaction.h"
#include "client.h"
#include "db.h"

namespace impl = ::libany::componentfsdbsrv;

::libany::rcpp::ClientHandler* impl::ComponentDBSrv::new_client()
{
	return new impl::Client(_db);
}


impl::ComponentDBSrv::ComponentDBSrv()
{
	_db = new impl::DB();
}

extern "C" ::libany::ios::Service* 
iosrvmod_componentdb_new(int argc, char* const* argv)
{
	argc = argc;
	argv = argv;
	return new impl::ComponentDBSrv();
}

