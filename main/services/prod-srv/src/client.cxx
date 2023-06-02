#include <libany/prodsrv/product.h>
#include <stdio.h>
#include "transaction.h"
#include "atomic.h"

using namespace productsrv;

class Client : public ::libany::rcpp::ClientHandler {
	public:
		virtual ::libany::rcpp::Transaction* new_transaction();
		virtual ::libany::rcpp::AtomicTransaction* new_atomic_transaction();
		
		virtual ~Client();
};

using namespace ::libany::productsrv;

::libany::rcpp::Transaction* Client::new_transaction()
{
	return new Transaction();
}

::libany::rcpp::AtomicTransaction* Client::new_atomic_transaction()
{
	return new AtomicTransaction();
}

::libany::rcpp::ClientHandler* ProductSrv::new_client()
{
	return new Client();
}


Client::~Client()
{
}

