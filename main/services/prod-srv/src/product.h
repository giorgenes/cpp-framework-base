#ifndef __product_h
#define __product_h

#include "productdb.h"
#include <libany/bxtp/document.h>
#include <stdio.h>

class ProductDB;
class Product {
	private:
		void write_next_version(const char*, int);
		void read_next_version(const char*, int*);
		void add_version(const char*);
		void add_child(const char*, const char*);
		FILE* open_lst_file(const char* rev, const char*);
		FILE* open_childs_file(const char* rev, const char*);
		FILE* open_versions_file(const char*);
		void send_file(
				const char*, 
				const char*, 
				const char*, 
				::libany::bxtp::ODocument&);
		void send_childs(
				const char*, 
				::libany::bxtp::ODocument&);
	public:
		ProductDB& db;
		char name[128];

		bool exists();
		void create();
		void commit(char*, int, const char*, const char*);

		Product(ProductDB& d, const char* n)
			: db(d) 
		{
			strcpy(name, n);	
		};
		
		Product(ProductDB& d)
			: db(d) 
		{
			*name = 0;
		};

		void send_files(const char*, ::libany::bxtp::ODocument&);
		void send_versions(::libany::bxtp::ODocument&);

};

#endif
