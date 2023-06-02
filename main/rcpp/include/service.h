#ifndef __libany__rcpp__service_h
#define __libany__rcpp__service_h

#include <libany/ios/ios.h>

namespace libany {
	namespace rcpp {
		class Transaction {
			public:
				virtual void commit() = 0;
				virtual void rollback() = 0;
				virtual void end() = 0;
				virtual void begin() = 0;
				virtual void exec() = 0;

				virtual ~Transaction();
		};

		class Client {
			public:
				virtual Transaction* new_transaction() = 0;
				virtual void begin() = 0;
				virtual void end() = 0;
				virtual void exec() = 0;

				virtual ~Client();

		};
		
		class Service : public ::libany::ios::Service {
			public:
				void handle_client(::libany::stream::IOStream*);
				virtual Client* new_client() = 0;
		};
	}
}

#endif
