#include "srv.h"
#include <stdio.h>
#include "client.h"
#include "db.h"

using namespace ::libany::postgresrv;
namespace impl = postgresrv_impl;

::libany::rcpp::ClientHandler* PostgreSrv::new_client()
{
	return new impl::Client(_db);
}

PostgreSrv::PostgreSrv()
{
	_db = new impl::PostgreDB();
}

PostgreSrv::~PostgreSrv()
{
	if(_db) {
		delete _db;
	}
}


