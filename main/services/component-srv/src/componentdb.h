#ifndef __componentdb_h
#define __componentdb_h

#include <string.h>
#include "component.h"
#include <stdio.h>
#include <libany/rcpp/session.h>
#include "db.h"


class Component;
class ComponentDB {
	private:
		componentsrv_impl::DB* _db;
	public:
		char base[128];
		char brand[128];

		ComponentDB(componentsrv_impl::DB* db, 
				const char* _brand, const char* _base)
			: _db(db) {
			strcpy(brand, _brand);
			strcpy(base, _base);
		};

		void write_components(::libany::bxtp::Document&);

		inline ::libany::rcpp::RcppStream& stream() {
			return *_db->_rcpp;
		};
		
		inline ::libany::strfile::StrFile& strfile() {
				return _db->strfile();
			};


		~ComponentDB();
};

#endif
