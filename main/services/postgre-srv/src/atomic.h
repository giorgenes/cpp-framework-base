#ifndef __atomictransaction_h
#define __atomictransaction_h

#include <libany/dbsrv/transaction.h>
#include "base.h"

namespace postgresrv_impl {
	
class AtomicTransaction : 
	public ::libany::rcpp::AtomicTransaction, virtual public BTransaction {
	public:
		
		virtual void begin();
		virtual void begin_exec(const char*, const char*);
	
		virtual void exec(::libany::bxtp::ODocument&);
		virtual void key(::libany::bxtp::IDocument&);
		virtual void data(::libany::bxtp::IDocument&);

		virtual ~AtomicTransaction();
		AtomicTransaction(PostgreDB* db) : BTransaction(db) {};
};

}

#endif
