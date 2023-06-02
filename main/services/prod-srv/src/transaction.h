#ifndef __transaction_h
#define __transaction_h

#include <libany/usersrv/user.h>

namespace productsrv {

class Transaction : public ::libany::rcpp::Transaction {
	public:
		virtual void commit();
		virtual void rollback();
		virtual void begin();

		virtual void exec(::libany::bxtp::ODocument&);
		virtual void key(::libany::bxtp::IDocument&);
		virtual void data(::libany::bxtp::IDocument&);

		virtual void begin_exec(const char*, const char*);

		virtual ~Transaction();
};

}

#endif
