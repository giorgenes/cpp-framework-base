#include "md.h"
#include <libany/utils/string_utils.h>
#include <libany/utils/path_utils.h>
#include <stdio.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace md = ::libany::metadir;

void md::MetaDir::create()
{
	char path[1024];

	sprintf(path, "%s/%s", _path, _name);
	::libany::utils::path_create(path);
}

void md::MetaDir::find(char* path)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));

	char buf[1024];
	char* p;
	int r;
	struct stat st;
	do {
		sprintf(buf, "%s/%s", cwd, _name);
		r = stat(buf, &st);
		if(r == 0) {
			strcpy(path, cwd);
		}
		else {
			p = strrchr(cwd, '/');
			if(p) *p = 0;
		}
	}while(r != 0 && p);

	if(r) {
		throw std::runtime_error("could not find mdir struct");
	}

}

md::MetaDir::MetaDir(const char* name, const char* path)
{
	strcpy(_name, name);
	
	if(path && *path) {
		strcpy(_path, path);
	}
	else {
		find(_path);
	}

	::libany::utils::getrealpath(_path, sizeof(_path));
}

void md::MetaDir::read_single_value(const char* key, char* value)
{
	char path[1024];
	sprintf(path, "%s/%s/%s.txt", _path, _name, key);
	FILE *fp = fopen(path, "r");
	if(!fp) {
		throw std::runtime_error("could not read revision");
	}
	fscanf(fp, "%s", value);
	fclose(fp);
}

void 
md::MetaDir::write_single_value(const char* key, const char* value)
{
	char path[1024];
	sprintf(path, "%s/%s/%s.txt", _path, _name, key);
	FILE *fp = fopen(path, "w");
	if(!fp) {
		throw std::runtime_error("could not write revision");
	}
	fprintf(fp, "%s\n", value);
	fclose(fp);
}

void md::MetaDir::get_key_path(const char* key, char* path)
{
	sprintf(path, "%s/%s/%s.txt", _path, _name, key);
}

