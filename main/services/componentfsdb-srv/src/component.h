#ifndef __component_h
#define __component_h

#include <libany/bxtp/document.h>
#include <stdio.h>
#include "db.h"
#include <cstring>

namespace libany {
	namespace componentfsdbsrv {
		class Component {
			private:
				char __base[128];
				char __brand[128];
				DB* __db;

				
				void read_next_revision(const char*, int*);
				void add_child(const char*, const char*);
				FILE* open_lst_file(const char* rev, const char*);
				FILE* open_childs_file(const char* rev, const char*);
				FILE* open_revisions_file(const char*);
				FILE* open_db_file(const char* file, const char* mode);
				void send_file(
						const char*, 
						const char*, 
						const char*, 
						::libany::bxtp::ODocument&);
				void send_childs(
						const char*, 
						::libany::bxtp::ODocument&);
			public:
				char name[128];

				bool exists();
				void create();
				void commit(char*, int, const char*, const char*);

				void send_files(const char*, ::libany::bxtp::ODocument&);
				void send_revisions(::libany::bxtp::ODocument&);
				void add_revision(const char*);
				void write_next_revision(const char*, int);
				void write_components(::libany::bxtp::ODocument& doc);

				Component(DB* db, const char* n)
					: __db(db) {
						std::strcpy(name, n);
						strcpy(__base, "default");
						strcpy(__brand, "default");
				};

		};
	}
}

#endif
