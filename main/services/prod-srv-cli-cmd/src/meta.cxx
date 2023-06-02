#include "meta.h"
#include <libany/utils/string_utils.h>
#include <libany/utils/path_utils.h>
#include <stdio.h>
#include <stdexcept>

void 
MetaDir::load_file_list(
		std::vector< cli::ComponentInfo>* files)
{
	char path[1024];

	sprintf(path, "%s/VSHP/files.txt", _path);
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
		files->push_back(cli::ComponentInfo(buf, p));
	}
	fclose(fp);

}

void 
MetaDir::save_file_list(
		std::vector< cli::ComponentInfo>& files)
{
	char buf[1024];
	
	sprintf(buf, "%s/VSHP/files.txt", _path);
	FILE* fp = fopen(buf, "w");
	if(!fp) {
		std::runtime_error("cant' create files");
	}
	for(unsigned int i=0; i<files.size(); i++) {
		fprintf(fp, "%s;%s\n", files[i].name, files[i].revision);
	}
	fclose(fp);
}

void MetaDir::write_version(const char* rev)
{
	char path[1024];
	sprintf(path, "%s/VSHP/version.txt", _path);
	FILE *fp = fopen(path, "w");
	if(!fp) {
		throw std::runtime_error("could not write version");
	}
	fprintf(fp, "%s\n", rev);
	fclose(fp);
}

void MetaDir::read_version(char* rev, int size)
{
	char path[1024];
	sprintf(path, "%s/VSHP/version.txt", _path);
	FILE *fp = fopen(path, "r");
	if(!fp) {
		throw std::runtime_error("could not read version");
	}
	fscanf(fp, "%s", rev);
	fclose(fp);
}

void MetaDir::create()
{
	char path[1024];

	sprintf(path, "%s/VSHP", _path);
	::libany::utils::path_create(path);
}

void MetaDir::find()
{
}

MetaDir::MetaDir(const char* path)
{
	if(path) {
		strcpy(_path, path);
	}
	else {
		find();
	}
}


