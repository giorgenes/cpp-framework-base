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
		class BTransaction : public virtual ::libany::dbsrv::BTransaction {
			private:
			protected:
				char _tmppath[1024];
				char _tmppath_filelist[1024];
				DB* _db;
			public:
				virtual void data(::libany::bxtp::IDocument&);

				BTransaction(DB* db)
					: _db(db) {}

				virtual ~BTransaction();
				static bool check_param(
						const char* name, const char* check, int n, int desired);

		};
	}
}

#endif
