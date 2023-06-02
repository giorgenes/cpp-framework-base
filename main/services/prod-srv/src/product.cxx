#include "product.h"
#include <stdexcept>
#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <sys/stat.h>
#include <sys/types.h>

static void 
get_next_version(const char* rev, char* next, int iter)
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

FILE* Product::open_versions_file(const char* mode)
{
	char buf[1024];
	sprintf(buf, "/tmp/productsrvdb/%s.%s/%s/versions.txt", 
			db.brand, db.base, name);
	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("cant open versions file");
	}

	return fp;
}

void Product::add_version(const char* rev)
{
	FILE* fp = open_versions_file("a");
	fprintf(fp, "%s\n", rev);
	fclose(fp);
}

FILE* Product::open_childs_file(const char* rev, const char* mode)
{
	char buf[1024];
	sprintf(buf, "/tmp/productsrvdb/%s.%s/%s/%s.childs", 
			db.brand, db.base, name, rev);
	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("cant open childs file");
	}

	return fp;
}

void Product::add_child(const char* rev, const char* child)
{
	FILE* fp = open_childs_file(rev, "a");
	fprintf(fp, "%s\n", child);
	fclose(fp);
}

void Product::create()
{
	try {
		if(exists()) {
			throw std::runtime_error("product exists");
		}
		FILE* fp;
		char buf[1024];

		sprintf(buf, "/tmp/productsrvdb/%s.%s.txt", db.brand, db.base);
		fp = fopen(buf, "a");
		if(!fp) {
			throw std::runtime_error("cant open productsrvdb!");
		}
		fprintf(fp, "%s\n", name);
		fclose(fp);

		sprintf(buf, "/tmp/productsrvdb/%s.%s", db.brand, db.base);
		mkdir(buf, 0700);
		
		sprintf(buf, "/tmp/productsrvdb/%s.%s/%s", db.brand, db.base, name);
		mkdir(buf, 0700);
		
		add_version("0");

		write_next_version("0", 0);
	}
	catch(...) {
		throw;
	}
}

bool Product::exists()
{
	FILE* fp;
	char buf[1024];

	sprintf(buf, "/tmp/productsrvdb/%s.%s.txt", db.brand, db.base);
	fp = fopen(buf, "r");
	if(!fp) {
		throw std::runtime_error("cant open productsrvdb");
	}

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

FILE* Product::open_lst_file(const char* rev, const char* mode)
{
	char buf[1024];

	sprintf(buf, "/tmp/productsrvdb/%s.%s/%s/%s.lst",
			db.brand, db.base, name, rev);

	FILE* fp = fopen(buf, mode);
	if(!fp) {
		throw std::runtime_error("could not open lst file");
	}
	return fp;
}

void 
Product::send_file(
		const char* id,
		const char* path,
		const char* rev, 
		::libany::bxtp::ODocument& doc)
{
	doc.write_tag("name", id);
	doc.write_tag("revision", path);
}

void 
Product::send_files(const char* rev, ::libany::bxtp::ODocument& doc)
{
	/* the first version has no files */
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
		doc.begin("component");
		send_file(buf, p, rev, doc);
		doc.end();
	}
	fclose(fp);
}

void Product::write_next_version(const char* rev, int iter)
{
	char buf[1024];
	sprintf(buf, "/tmp/productsrvdb/%s.%s/%s/%s.next", 
				db.brand, db.base, name, rev);

	FILE* fp = fopen(buf, "w");
	fprintf(fp, "%d\n", iter);
	fclose(fp);
}

void Product::read_next_version(const char* rev, int* iter)
{
	char buf[1024];
	sprintf(buf, "/tmp/productsrvdb/%s.%s/%s/%s.next", 
				db.brand, db.base, name, rev);

	FILE* fp = fopen(buf, "r");
	fscanf(fp, "%d", iter);
	fclose(fp);
}

void 
Product::commit(
		char* rev, 
		int revsize, 
		const char* path, 
		const char* path_filelist)
{
	char next[1024];
	
	int iter;
	read_next_version(rev, &iter);
	get_next_version(rev, next, iter);

	char buf[1024];
	sprintf(buf, "/tmp/productsrvdb/%s.%s/%s/%s.lst",
			db.brand, db.base, name, next);
	if(rename(path_filelist, buf) != 0) {
		throw std::runtime_error("could not mv path");
	}

	add_version(next);
	add_child(rev, next);
	
	iter++;
	write_next_version(rev, iter);
	write_next_version(next, 0);
	strcpy(rev, next);
}

void 
Product::send_childs(const char* rev, ::libany::bxtp::ODocument& doc)
{
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

void 
Product::send_versions(::libany::bxtp::ODocument& doc)
{
	FILE* fp = open_versions_file("r");
	char buf[1024];
	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		::libany::utils::string_trimall(buf, 0);
		doc.begin("version");
		doc.write_tag("number", buf);
		//fprintf(stderr, "sending %s\n", buf);
		doc.begin("childs");
		send_childs(buf, doc);
		doc.end();
		doc.end();
	}
	fclose(fp);
}


