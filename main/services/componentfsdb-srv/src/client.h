#ifndef __client_h
#define __client_h

#include "srv.h"
#include <libany/rcpp/session.h>
#include <libany/stfactory/uristream.h>
#include <stdio.h>
#include "transaction.h"
#include "db.h"

namespace libany {
	namespace componentfsdbsrv {
		class Client : public ::libany::rcpp::ClientHandler {
			private:
				DB* _db;
			public:
				virtual ::libany::rcpp::Transaction* new_transaction();

				Client(DB*);
				virtual ~Client();
		};
	}
}

#endif

