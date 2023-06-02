#ifndef __basetransaction_h
#define __basetransaction_h

#include <libany/componentsrv/component.h>

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

namespace componentsrv_impl {
	class Transaction : public virtual ::libany::rcpp::Transaction {
		private:
			void get_files(::libany::bxtp::Document&);
			void get_file(::libany::bxtp::Document&, FILE*);
			void get_revision(const char*, ::libany::bxtp::Document&);
		protected:
			char _base[128];
			char _brand[128];
			char _name[128];
			char _revision[128];
			int _oper;
			int _param;
			char _tmppath[1024];
			char _tmppath_filelist[1024];
			DB* _db;
		public:
			virtual void begin();
			virtual void begin_exec(const char*, const char*);

			virtual void exec(::libany::bxtp::Document&);
			virtual void nexec(::libany::bxtp::Document&);
			virtual void key(::libany::bxtp::Document&);
			virtual void data(::libany::bxtp::Document&);

			virtual void commit();
			virtual void rollback();


			Transaction(DB* db)
				: _db(db) {}

			virtual ~Transaction();
	};

}

#endif
