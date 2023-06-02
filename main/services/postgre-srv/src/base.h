#ifndef __basetransaction_h
#define __basetransaction_h

#include "srv.h"
#include "db.h"
#include <stdio.h>

#include <libany/dbsrv/transaction.h>

namespace postgresrv_impl {
	class BTransaction : public ::libany::dbsrv::BTransaction {
		protected:
			PostgreDB* _db;
		public:
			BTransaction(PostgreDB*);
			virtual ~BTransaction();

			virtual void data(::libany::bxtp::IDocument& doc);
	};

}

#endif
