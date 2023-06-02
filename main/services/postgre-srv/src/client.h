#ifndef __client_h
#define __client_h

#include "srv.h"
#include <stdio.h>
#include "transaction.h"
#include "atomic.h"
#include "db.h"

namespace postgresrv_impl {
	class Client : public ::libany::rcpp::ClientHandler {
		private:
			PostgreDB* _db;
		public:
			virtual ::libany::rcpp::Transaction* new_transaction();
			virtual ::libany::rcpp::AtomicTransaction* new_atomic_transaction();

			Client(PostgreDB* db) : _db(db) {
			};

			virtual ~Client();
	};
}


#endif
