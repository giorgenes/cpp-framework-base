#ifndef __component_h
#define __component_h

#include "componentdb.h"
#include <libany/bxtp/document.h>
#include <stdio.h>

class ComponentDB;
class Component {
	private:
		void write_next_revision(const char*, int);
		void read_next_revision(const char*, int*);
		void add_revision(const char*);
		void add_child(const char*, const char*);
		FILE* open_lst_file(const char* rev, const char*);
		FILE* open_childs_file(const char* rev, const char*);
		FILE* open_revisions_file(const char*);
		void send_file(
				::libany::stream::Stream&,
				const char*, 
				const char*, 
				const char*, 
				::libany::bxtp::Document&);
		void send_childs(
				const char*, 
				::libany::bxtp::Document&);
	public:
		ComponentDB& _db;
		char name[128];

		bool exists();
		void create();
		void commit(char*, int, const char*, const char*);

		Component(ComponentDB& d, const char* n)
			: _db(d) 
		{
			strcpy(name, n);	
		};
		
		Component(ComponentDB& d)
			: _db(d) 
		{
			*name = 0;
		};

		void send_files(const char*, ::libany::bxtp::Document&);
		void send_revisions(::libany::bxtp::Document&);

};

#endif
