#include "component.h"
#include <stdexcept>
#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libany/dbsrvcli/db.h>

static void 
get_next_revision(const char* rev, char* next, int iter)
{
	if(strcmp(rev, "0")==0) {
		sprintf(next, "%d.1", iter);
	}
	else {
		char* p;
		if(iter == 0) {
			p = strrchr(rev, '.');
			p++;
			strncpy(next, rev, p - rev);
			int val = atoi(p);
			sprintf(next + (p-rev), "%d", val+1);
		}
		else {
			sprintf(next, "%s.%d.1", rev, iter-1);
		}
#if 0
		else {
			strcpy(next, rev);
			p = strrchr(next, '.');
			*p = 0;
			p = strrchr(next, '.');
			sprintf(p, "%d.1", iter-1);
		}
#endif
	}
	fprintf(stderr, "%s(%d) = %s\n", rev, iter, next);
}

FILE* Component::open_revisions_file(const char* mode)
{
	char buf[1024];
	sprintf(buf, "/tmp/componentsrvdb/%s.%s/%s/revisions.txt", 
			_db.brand, _db.base, name);
	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("cant open revisions file");
	}

	return fp;
}

void Component::add_revision(const char* rev)
{
	char buf[1024];
	::libany::dbsrvcli::DB db(_db.stream(), "default");
	_db.strfile().loadstr("add_revision");
	sprintf(buf, _db.strfile().str(), name, rev);
	::libany::table::Table table;
	db.exec(buf, table);
}

FILE* Component::open_childs_file(const char* rev, const char* mode)
{
	char buf[1024];
	sprintf(buf, "/tmp/componentsrvdb/%s.%s/%s/%s.childs", 
			_db.brand, _db.base, name, rev);
	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("cant open childs file");
	}

	return fp;
}

void Component::add_child(const char* rev, const char* child)
{
	FILE* fp = open_childs_file(rev, "a");
	fprintf(fp, "%s\n", child);
	fclose(fp);
}

void Component::create()
{
	if(exists()) {
		throw std::runtime_error("component exists");
	}
	
	char buf[1024];
	::libany::dbsrvcli::DB db(_db.stream(), "default");
	_db.strfile().loadstr("create_component");
	sprintf(buf, _db.strfile().str(), name);
	::libany::table::Table table;
	db.exec(buf, table);

	add_revision("0");
	write_next_revision("0", 0);
}

bool Component::exists()
{
	return false;
}

FILE* Component::open_lst_file(const char* rev, const char* mode)
{
	char buf[1024];

	sprintf(buf, "/tmp/componentsrvdb/%s.%s/%s/%s.lst",
			_db.brand, _db.base, name, rev);

	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("could not open lst file");
	}
	return fp;
}

void 
Component::send_file(
		::libany::stream::Stream& in,
		const char* id,
		const char* path,
		const char* rev, 
		::libany::bxtp::Document& doc)
{
	doc.write_tag("id", id);
	doc.write_tag("path", path);
	doc.begin("content");
	char buf[1024];
	int len;
	while((len=in.read(buf, sizeof(buf))) > 0) {
		doc.write(buf, len);
	}

	doc.end();
}

void 
Component::send_files(const char* rev, ::libany::bxtp::Document& doc)
{
	/* the first revision has no files */
	if(strcmp(rev, "0")==0) {
		return;
	}

	char buf[1024];
	::libany::dbsrvcli::DB db(_db.stream(), "default");
	_db.strfile().loadstr("get_revision_listing");
	sprintf(buf, _db.strfile().str(), name, rev);
	::libany::table::Table table;
	db.exec(buf, table);

	_db.strfile().loadstr("get_revision_file");
	for(int i = 0; i<table.ntuples(); i++) {	
		sprintf(buf, _db.strfile().str(), 
				name, rev, table.field_value(i, 0));
	
		/* gets a blob into _db.stream() */
		db.exec(buf);
		doc.begin("file");
		send_file(_db.stream(), 
				table.field_value(i, 0),
				table.field_value(i, 1), 
				rev, 
				doc);
		doc.end();
	}
}

void Component::write_next_revision(const char* rev, int iter)
{
	char buf[1024];
	::libany::dbsrvcli::DB db(_db.stream(), "default");
	_db.strfile().loadstr("set_next_revision");
	sprintf(buf, _db.strfile().str(), name, rev, iter);
	::libany::table::Table table;
	db.exec(buf, table);
}

void Component::read_next_revision(const char* rev, int* iter)
{
	char buf[1024];
	sprintf(buf, "/tmp/componentsrvdb/%s.%s/%s/%s.next", 
				_db.brand, _db.base, name, rev);

	FILE* fp = fopen(buf, "r");
	fscanf(fp, "%d", iter);
	fclose(fp);
}

void 
Component::commit(
		char* rev, 
		int revsize, 
		const char* path, 
		const char* path_filelist)
{
	char next[1024];
	
	int iter;
	read_next_revision(rev, &iter);
	get_next_revision(rev, next, iter);

	char buf[1024];
	sprintf(buf, "/tmp/componentsrvdb/%s.%s/%s/%s",
			_db.brand, _db.base, name, next);

	if(rename(path, buf) != 0) {
		throw std::runtime_error("could not mv path");
	}

	sprintf(buf, "/tmp/componentsrvdb/%s.%s/%s/%s.lst",
			_db.brand, _db.base, name, next);
	if(rename(path_filelist, buf) != 0) {
		throw std::runtime_error("could not mv path");
	}

	add_revision(next);
	add_child(rev, next);
	
	iter++;
	write_next_revision(rev, iter);
	write_next_revision(next, 0);
	strcpy(rev, next);
}

void 
Component::send_childs(const char* rev, ::libany::bxtp::Document& doc)
{
	try {
		FILE* fp = open_childs_file(rev, "r");
		char buf[1024];
		int len;
		while(fgets(buf, sizeof(buf), fp)!=NULL) {
			len = strlen(buf);
			::libany::utils::string_trimall(buf, &len);
			doc.write(buf, len);
			//fprintf(stderr, "sending-- %s\n", buf);
			*buf = ' ';
			buf[1] = 0;
			doc.write(buf, 1);
		}
		fclose(fp);
	}
	catch(...) {
	}
}

void 
Component::send_revisions(::libany::bxtp::Document& doc)
{
	FILE* fp = open_revisions_file("r");
	char buf[1024];
	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		::libany::utils::string_trimall(buf, 0);
		doc.begin("revision");
		doc.write_tag("number", buf);
		//fprintf(stderr, "sending %s\n", buf);
		doc.begin("childs");
		send_childs(buf, doc);
		doc.end();
		doc.end();
	}
	fclose(fp);
}


