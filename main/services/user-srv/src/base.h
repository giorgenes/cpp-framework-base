#ifndef __basetransaction_h
#define __basetransaction_h

#include <libany/usersrv/user.h>

#define PARAM_NAME  0
#define PARAM_BRAND 1
#define PARAM_BASE  2

#define OPER_CREATE 0
#define OPER_CHECK  1
#define OPER_LIST   2

#include "user.h"

namespace usersrv_impl {

class BaseTransaction : public virtual ::libany::rcpp::BaseTransaction {
	protected:
		char _base[128];
		char _brand[128];
		char _name[128];
		int _oper;
		int _param;
	public:
		virtual void begin();
		
		virtual void exec(::libany::bxtp::ODocument&);
		virtual void key(::libany::bxtp::IDocument&);
		virtual void data(::libany::bxtp::IDocument&);

		virtual void begin_exec(const char*, const char*);

		virtual ~BaseTransaction();
};

}

#endif
