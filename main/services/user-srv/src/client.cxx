#include <libany/usersrv/user.h>
#include <stdio.h>
#include "transaction.h"
#include "atomic.h"

class Client : public ::libany::rcpp::ClientHandler {
	public:
		virtual ::libany::rcpp::Transaction* new_transaction();
		virtual ::libany::rcpp::AtomicTransaction* new_atomic_transaction();
		
		virtual ~Client();
};

using namespace ::libany::usersrv;
using namespace usersrv_impl;

::libany::rcpp::Transaction* Client::new_transaction()
{
	return new Transaction();
}

::libany::rcpp::AtomicTransaction* Client::new_atomic_transaction()
{
	return new AtomicTransaction();
}

::libany::rcpp::ClientHandler* UserSrv::new_client()
{
	return new Client();
}


Client::~Client()
{
}

extern "C" {

::libany::ios::Service* 
iosrvmod_user_new(int argc, char* const* argv)
{
	argc = argc;
	argv = argv;
	return new UserSrv();
}

}
