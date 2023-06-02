#ifndef __atomictransaction_h
#define __atomictransaction_h

#include <libany/usersrv/user.h>
#include "base.h"

namespace usersrv_impl {

class AtomicTransaction : 
	public virtual ::libany::rcpp::AtomicTransaction, public BaseTransaction {
	public:
		virtual void begin();
		
		virtual void exec(::libany::bxtp::ODocument&);
		virtual void key(::libany::bxtp::IDocument&);
		virtual void data(::libany::bxtp::IDocument&);

		virtual void begin_exec(const char*, const char*);

		virtual ~AtomicTransaction();
};

}

#endif
