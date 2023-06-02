#ifndef __client_h
#define __client_h

#include "srv.h"
#include <libany/rcpp/session.h>
#include <libany/stfactory/uristream.h>
#include <stdio.h>
#include "transaction.h"
#include "atomic.h"
#include "db.h"

namespace libany {
	namespace componentfsdbsrv {
		class Client : public ::libany::rcpp::ClientHandler {
			private:
				DB* _db;
			public:
				virtual ::libany::rcpp::Transaction* new_transaction();
				virtual ::libany::rcpp::AtomicTransaction* new_atomic_transaction();

				Client(DB*);
				virtual ~Client();
		};
	}
}

#endif

