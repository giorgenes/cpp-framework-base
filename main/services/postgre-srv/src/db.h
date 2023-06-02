#ifndef __db_h
#define __db_h

#include <postgresql/libpq-fe.h>

namespace postgresrv_impl {
	class PostgreDB {
		private:
		public:
			void commit();
			void begin_transaction();

			PostgreDB();
			~PostgreDB();
			
			PGconn *_conn;
	};
}

#endif
