#ifndef __libany__rcpp__service_h
#define __libany__rcpp__service_h

#include <libany/ios/ios.h>
#include <libany/rcpp/session.h>
#include <libany/bxtp/document.h>

namespace libany {
	namespace rcpp {
		class Transaction {
			public:
				virtual void begin();
				virtual void begin_exec(const char*, const char*);
				virtual void key(::libany::bxtp::Document&);
				virtual void data(::libany::bxtp::Document&);
				virtual void exec(::libany::bxtp::Document&);
				virtual void nexec(::libany::bxtp::Document&);

				virtual ~Transaction() {};
				virtual void commit() = 0;
				virtual void rollback() = 0;
		};

		class ClientHandler {
			public:
				virtual Transaction* new_transaction() = 0;

				virtual ~ClientHandler();

		};
		
		class Service : public ::libany::ios::Service {
			private:
				void create_trans(ClientHandler*, Transaction*&);
				void exec(Transaction*, ::libany::bxtp::Document&, bool);
			protected:
			public:
				void handle_client(::libany::stream::Stream*);
				virtual ClientHandler* new_client() = 0;

				virtual ~Service();
		};

		class Client : public ::libany::ios::Client {
			protected:
				RcppStream& _rcppstream;
			public:
				Client(RcppStream& st) 
				: ::libany::ios::Client(st), _rcppstream(st) {
				};
		};
	}
}

#endif
