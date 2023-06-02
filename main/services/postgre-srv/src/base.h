#ifndef __basetransaction_h
#define __basetransaction_h

#include "srv.h"
#include "db.h"
#include <stdio.h>

#include <libany/dbsrv/transaction.h>

namespace postgresrv_impl {
	class Transaction : public ::libany::dbsrv::Transaction {
		protected:
			PostgreDB* _db;
		public:
			Transaction(PostgreDB*);
			virtual ~Transaction();

			virtual void data(::libany::bxtp::Document& doc);
			virtual void commit();
			virtual void rollback();
			virtual void begin();
			virtual void exec(::libany::bxtp::Document&);
			virtual void key(::libany::bxtp::Document&);
			virtual void begin_exec(const char*, const char*);
	};

}

#endif
