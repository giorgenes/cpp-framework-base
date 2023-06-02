#include <libany/componentsrv/component.h>
#include <libany/rcpp/session.h>
#include <libany/stfactory/uristream.h>
#include <stdio.h>
#include "transaction.h"
#include "atomic.h"
#include "client.h"
#include "db.h"

namespace impl {
	using namespace componentsrv_impl;
	using namespace ::libany::componentsrv;
}

::libany::rcpp::ClientHandler* impl::ComponentSrv::new_client()
{
	return new impl::Client(_db);
}


impl::ComponentSrv::ComponentSrv()
{
	try {
		_db = new impl::DB();
	}
	catch(...) {
		fprintf(stderr, "coudl not creqte db\n");
		throw;
	}
}

