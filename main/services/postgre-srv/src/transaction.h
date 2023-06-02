#ifndef __transaction_h
#define __transaction_h

#include <postgresql/libpq-fe.h>
#include "srv.h"
#include "base.h"
#include <libany/dbsrv/transaction.h>

namespace postgresrv_impl {
	class Transaction : 
		public virtual ::libany::rcpp::Transaction, public BTransaction {
			private:
			public:
				virtual void commit();
				virtual void rollback();
				virtual void begin();

				virtual void exec(::libany::bxtp::ODocument&);
				virtual void key(::libany::bxtp::IDocument&);
				virtual void data(::libany::bxtp::IDocument&);

				virtual void begin_exec(const char*, const char*);

				virtual ~Transaction();
				Transaction(PostgreDB* db) : BTransaction(db) {};
		};

}

#endif
