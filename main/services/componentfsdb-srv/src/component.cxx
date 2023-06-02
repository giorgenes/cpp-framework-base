#include "component.h"
#include <stdexcept>
#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <libany/utils/path_utils.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libany/dbsrvcli/db.h>

using namespace ::libany::componentfsdbsrv;

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
	sprintf(buf, "%s/%s.%s/%s/revisions.txt", 
			__db->root(), __brand, __base, name);
	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("cant open revisions file");
	}

	return fp;
}

void Component::add_revision(const char* rev)
{
	FILE* fp = open_revisions_file("a");
	fprintf(fp, "%s\n", rev);
	fclose(fp);
}

FILE* Component::open_childs_file(const char* rev, const char* mode)
{
	char buf[1024];
	sprintf(buf, "%s/%s.%s/%s/%s.childs", 
			__db->root(), __brand, __base, name, rev);
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
	
	sprintf(buf, "%s/%s.%s/%s", __db->root(), __brand, __base, name);
	fprintf(stderr, "criando %s\n", buf);
	::libany::utils::path_create(buf);

	FILE* fp = open_db_file("components", "a+");
	fprintf(fp, "%s\n", name);
	fclose(fp);
}

FILE* Component::open_db_file(const char* file, const char* mode)
{
	char buf[1024];

	sprintf(buf, "%s/%s.%s_%s.txt",
			__db->root(), __brand, __base, file);

	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("could not open db file");
	}
	return fp;
}

bool Component::exists()
{
	FILE* fp;
	char buf[1024];

	fp = open_db_file("components", "r");

	char* p;
	bool e = false;
	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		p = ::libany::utils::string_trimall(buf, 0);
		if(strcmp(p, name)==0) {
			e = true;
			break;
		}
	}

	fclose(fp);
	
	return e;
}

FILE* Component::open_lst_file(const char* rev, const char* mode)
{
	char buf[1024];

	sprintf(buf, "%s/%s.%s/%s/%s.lst",
			__db->root(), __brand, __base, name, rev);

	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("could not open lst file");
	}
	return fp;
}

void 
Component::send_file(
		const char* id,
		const char* path,
		const char* rev, 
		::libany::bxtp::Document& doc)
{
	doc.write_tag("id", id);
	doc.write_tag("path", path);
	doc.begin("content");
	char buf[1024];
	sprintf(buf, "%s/%s.%s/%s/%s/%s",
			__db->root(), __brand, __base, name, rev, path);
	FILE* fp = fopen(buf, "r");
	int len;
	while((len=fread(buf, 1, sizeof(buf), fp))>0) {
		doc.write(buf, len);
	}
	fclose(fp);
	doc.end();
}

void 
Component::send_files(const char* rev, ::libany::bxtp::Document& doc)
{
	/* the first revision has no files */
	if(strcmp(rev, "0")==0) {
		return;
	}

	FILE* fp = open_lst_file(rev, "r");
	char buf[1024];
	char* p;
	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		::libany::utils::string_trimall(buf, 0);
		p = strchr(buf, ';');
		*p++ = 0;
		doc.begin("file");
		send_file(buf, p, rev, doc);
		doc.end();
	}
	fclose(fp);
}

void Component::write_next_revision(const char* rev, int iter)
{
	char buf[1024];
	sprintf(buf, "%s/%s.%s/%s/%s.next", 
				__db->root(), __brand, __base, name, rev);

	FILE* fp = fopen(buf, "w");
	fprintf(fp, "%d\n", iter);
	fclose(fp);
}

void Component::read_next_revision(const char* rev, int* iter)
{
	char buf[1024];
	sprintf(buf, "%s/%s.%s/%s/%s.next", 
				__db->root(), __brand, __base, name, rev);

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
	sprintf(buf, "%s/%s.%s/%s/%s",
			__db->root(), __brand, __base, name, next);

	if(rename(path, buf) != 0) {
		throw std::runtime_error("could not mv path");
	}

	sprintf(buf, "%s/%s.%s/%s/%s.lst",
			__db->root(), __brand, __base, name, next);
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

void Component::write_components(::libany::bxtp::Document& doc)
{
	FILE* fp = open_db_file("components", "r");
	char buf[1024];
	int n = 0;
	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		n++;
	}
	fseek(fp, SEEK_SET, 0);
	doc.begin("table");
	::libany::table::TableWriter w(doc, 1, n);
	w.write_field_name("name");
	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		::libany::utils::string_trimall(buf, 0);
		//fprintf(stderr, "---%s\n", buf);
		w.write_field_value(buf);
	}
	doc.end();
	fclose(fp);
}
