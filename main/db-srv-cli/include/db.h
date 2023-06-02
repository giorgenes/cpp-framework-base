#ifndef __libany__dbsrvcli__db_h
#define __libany__dbsrvcli__db_h

#include <libany/ios/ios.h>
#include <libany/stfactory/uristream.h>
#include <libany/rcpp/service.h>
#include <libany/table/table.h>
#include <vector>

namespace libany {
	namespace dbsrvcli {
		class DB : protected ::libany::rcpp::Client {
			private:
				char _db[1024];
			public:
				DB(::libany::rcpp::RcppStream&, const char*);

				void exec(const char*, ::libany::table::Table&);
				void exec(const char*);

				void nexec(const char*, ::libany::table::Table&);
				void nexec(const char*);
		};
	}
}

#endif
