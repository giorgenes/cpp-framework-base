#ifndef __basetransaction_h
#define __basetransaction_h

#include "srv.h"

#define PARAM_NAME  0
#define PARAM_BRAND 1
#define PARAM_BASE  2

#define OPER_NONE     0
#define OPER_CREATE   1
#define OPER_CHECK    2
#define OPER_LIST     3
#define OPER_CHECKOUT 4
#define OPER_COMMIT   5
#define OPER_LISTVER  6

#include "component.h"
#include <stdio.h>
#include "db.h"

#include <libany/dbsrv/transaction.h>

namespace libany {
	namespace componentfsdbsrv {
		class Transaction : public virtual ::libany::dbsrv::Transaction {
			private:
			protected:
				char _tmppath[1024];
				char _tmppath_filelist[1024];
				DB* _db;
			public:
				Transaction(DB* db)
					: _db(db) {}

				virtual ~Transaction();
				static bool check_param(
						const char* name, const char* check, int n, int desired);

				virtual void commit();
				virtual void rollback();
				virtual void begin();

				virtual void exec(::libany::bxtp::Document&);
				virtual void nexec(::libany::bxtp::Document&);
				virtual void key(::libany::bxtp::Document&);
				virtual void data(::libany::bxtp::Document&);

				virtual void begin_exec(const char*, const char*);


		};
	}
}

#endif
