#ifndef __atomictransaction_h
#define __atomictransaction_h

#include "base.h"

namespace libany {
	namespace componentfsdbsrv {
		class AtomicTransaction : 
			public virtual ::libany::rcpp::AtomicTransaction, public BTransaction {
				public:

					virtual void begin();
					virtual void begin_exec(const char*, const char*);

					virtual void exec(::libany::bxtp::ODocument&);
					virtual void key(::libany::bxtp::IDocument&);
					virtual void data(::libany::bxtp::IDocument&);

					virtual ~AtomicTransaction();

					AtomicTransaction(DB* db)
						: BTransaction(db) {};
			};
	}
}

#endif
