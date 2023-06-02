#ifndef __libany__rcpp__session_h
#define __libany__rcpp__session_h

#include <libany/ios/ios.h>
#include <libany/bxtp/stream.h>
#include <libany/bxtp/document.h>

#define LIBANY_RCCP_VER 1

namespace libany {
	namespace rcpp {
		class RcppStream : public ::libany::bxtp::BxtpStream {
			private:
				bool _exception;
			protected:
			public:
				RcppStream(::libany::stream::Stream &st)
					: ::libany::bxtp::BxtpStream(st) {
				};

				void commit();
				void rollback();
		};

		class RcppDocument : public ::libany::bxtp::Document {
			private:
				bool _keyclosed;
				bool _dataclosed;

				void throw_exception();
			protected:
			public:
				RcppDocument(RcppStream &st) 
					: ::libany::bxtp::Document(st) 
					{
					};

				void request_begin_nquery(const char*, const char* base = 0);
				void request_begin_query(const char*, const char* base = 0);
				
				void request_key(const char* key, 
						const char* value=0 , int len=0);

				void request_data(const char* key, 
						const char* value=0, int len=0);
				
				void request_end();

				void request_begin_data();
				
				virtual bool match(const char**, int*, bool e = false);
				virtual bool match(const char*, bool e = false);
				virtual void match_end();
		};

	}
}

#endif
