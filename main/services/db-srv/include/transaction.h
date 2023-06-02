#ifndef __libany__dbsrv__transaction_h
#define __libany__dbsrv__transaction_h

#include <libany/rcpp/service.h>

namespace libany {
	namespace dbsrv {
		class Transaction : public ::libany::rcpp::Transaction {
			private:
			protected:
				int __qlen;
				char* __query;
			public:
				virtual void data(::libany::bxtp::Document&);

				Transaction();
				virtual ~Transaction();

		};
		
	}
}

#endif
