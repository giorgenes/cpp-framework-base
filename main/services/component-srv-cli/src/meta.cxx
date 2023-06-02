#include "meta.h"
#include <libany/utils/string_utils.h>
#include <libany/utils/path_utils.h>
#include <stdio.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace md = ::libany::componentsrvcli;

void 
md::MetaDir::load_file_list(
		std::vector< ::libany::componentsrvcli::FileInfo>* files)
{
	char path[1024];

	get_key_path("files", path);
	FILE* fp = fopen(path, "r");
	if(!fp) {
		std::runtime_error("cant' open file list");
	}
	char buf[1024];
	char* p;
	while(fgets(buf, sizeof(buf), fp)!=NULL) {
		::libany::utils::string_trimall(buf, 0);
		p = strchr(buf, ';');
		if(!p) {
			throw std::runtime_error("invalid line");
		}
		*p++ = 0;
		files->push_back(::libany::componentsrvcli::FileInfo(buf, p));
	}
	fclose(fp);

}

void 
md::MetaDir::save_file_list(
		std::vector< ::libany::componentsrvcli::FileInfo>& files)
{
	char buf[1024];
	
	get_key_path("files", buf);
	FILE* fp = fopen(buf, "w");
	if(!fp) {
		throw std::runtime_error("cant' create files");
	}
	for(unsigned int i=0; i<files.size(); i++) {
		fprintf(fp, "%s;%s\n", files[i].id, files[i].path);
	}
	fclose(fp);
}

void md::MetaDir::write_revision(const char* rev)
{
	write_single_value("revision", rev);
}

void md::MetaDir::read_revision(char* rev, int size)
{
	read_single_value("revision", rev);
}

md::MetaDir::MetaDir(const char* path)
	: ::libany::metadir::MetaDir("VSH", path)
{
}

void md::MetaDir::read_name(char* name)
{
		read_single_value("name", name);
}

void md::MetaDir::write_name(const char* name)
{
	write_single_value("name", name);
}

void md::MetaDir::add(const char* path)
{
	char fpath[2048] = "./";
	const char* p = strrchr(path, '/');
	const char* fname;
	if(p) {
		strncpy(fpath, path, p - path);
		fpath[p-path] = 0;
		fname = p + 1;
	}
	else {
		fname = path;
	}
	::libany::utils::getrealpath(fpath, sizeof(fpath));

	int len = strlen(this->path());

	//fprintf(stderr, "path: %s\n", _path);
	//fprintf(stderr, "fpath: %s\n", fpath);
	if(strncmp(this->path(), fpath, len)!=0) {
		throw std::runtime_error("not a subpath");
	}
	char subpath[2048];
	strcpy(subpath, fpath + len + 1);
	strcat(subpath, "/");
	strcat(subpath, fname);

	char buf[1024];
	
	get_key_path("files", buf);
	FILE* fp = fopen(buf, "a");
	if(!fp) {
		throw std::runtime_error("cant' create files");
	}
	fprintf(fp, "%s;%s\n", fname, subpath);
	fclose(fp);

	fprintf(stderr, "added %s\n", subpath);
}

