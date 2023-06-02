#ifndef __libany__dbsrv__transaction_h
#define __libany__dbsrv__transaction_h

#include <libany/rcpp/service.h>

namespace libany {
	namespace dbsrv {
		class BTransaction : public ::libany::rcpp::BaseTransaction {
			private:
			protected:
				int __qlen;
				char* __query;
			public:
				virtual void data(::libany::bxtp::IDocument&);

				BTransaction();
				virtual ~BTransaction();

		};
		
		class AtomicTransaction : 
			public ::libany::rcpp::AtomicTransaction, public BTransaction {
				public:
					virtual ~AtomicTransaction();
			};

		class Transaction : 
			public virtual ::libany::rcpp::Transaction, public BTransaction {
				private:
				public:
					virtual ~Transaction();
					Transaction() : BTransaction() {};
			};
	}
}

#endif
