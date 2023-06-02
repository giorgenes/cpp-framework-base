#ifndef __db_h
#define __db_h

#include <libany/rcpp/session.h>
#include <libany/stfactory/uristream.h>
#include <libany/strfile/strfile.h>

namespace componentsrv_impl {
	class DB {
		private:
			::libany::stfactory::URIStream *_stream;
			::libany::strfile::StrFile _strfile;
			char __base_uri[1024];
			char __strfile_path[1024];

			void read_config(const char*);
		public:
			void commit();
			void begin_transaction();

			DB();
			~DB();
			::libany::rcpp::RcppStream *_rcpp;

			inline ::libany::strfile::StrFile& strfile() {
				return _strfile;
			};
	};
}

#endif
