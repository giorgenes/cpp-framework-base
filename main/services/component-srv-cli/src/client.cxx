#include <libany/componentsrvcli/component.h>
#include <stdexcept>
#include <libany/stfactory/uristream.h>
#include <memory>
#include <libany/utils/path_utils.h>

using namespace ::libany::componentsrvcli;

Component::Component(::libany::rcpp::RcppStream& r, 
		const char* name, 
		const char* brand, 
		const char* service)
	: ::libany::rcpp::Client(r)
{
	strcpy(_name, name);
	strcpy(_brand, brand);
	strcpy(_service, service);
}

void Component::create()
{
	try {
		::libany::rcpp::RcppDocument cmd(_rcppstream);
		
		cmd.request_begin_query("component");
		cmd.request_key("name", _name);
		cmd.request_key("brand", _brand);
		cmd.request_key("base", _service);
		cmd.request_data("create");
		cmd.request_end();

		cmd.match_end();
	}
	catch(...) {
		throw;
	}
}

static void 
create_file(const char* path, ::libany::stream::IStream& content)
{
	char dir[1024];
	const char* p = strrchr(path, '/');
	if(p) {
		strncpy(dir, path, p - path);
		dir[p-path] = 0;
		::libany::utils::path_create(dir);
	}

	FILE* fp = fopen(path, "w");
	if(!fp) {
		throw std::runtime_error("could not open file for writting");
	}

	char buf[1024];
	int len;
	while((len=content.read(buf, sizeof(buf))) > 0) {
		fwrite(buf, 1, len, fp);
	}

	fclose(fp);
	fprintf(stderr, "criando arquivo %s\n", path);
}

void 
Component::checkout(
		const char* path, 
		const char* revision, 
		std::vector<FileInfo>* files)
{
	try {
		::libany::rcpp::RcppDocument cmd(_rcppstream);

		cmd.request_begin_query("component");
		cmd.request_key("name", _name);
		cmd.request_key("brand", _brand);
		cmd.request_key("base", _service);

		cmd.request_begin_data();
		cmd.begin("checkout");
		cmd.write_tag("revision", revision);
		cmd.end();
		cmd.request_end();

		::libany::utils::path_create(path);
		int len;
		char buf[2048];
		while(cmd.match("/response/file")) {
			if(!cmd.match("/response/file/id")) {
				break;
			}
			char id[1024];
			len = cmd.read(id, sizeof(id)-1);
			id[len] = 0;

			cmd.match("/response/file/path");
			char rpath[1024];
			len = cmd.read(rpath, sizeof(rpath)-1);
			rpath[len] = 0;
			
			files->push_back(FileInfo(id, rpath));
			
			cmd.match("/response/file/content");
			sprintf(buf, "%s/%s", path, rpath);
			create_file(buf, cmd);
		}
	}
	catch(...) {
		throw;
	}

}

static void
send_file(const char* path, FileInfo& info, ::libany::stream::OStream& o)
{
	char buf[2048];
	sprintf(buf, "%s/%s", path, info.path);
	FILE* fp = fopen(buf, "r");
	if(!fp) {
		throw std::runtime_error("could not open file (send_file)");
	}

	int len;
	while((len = fread(buf, 1, sizeof(buf), fp)) > 0) {
		o.write(buf, len);
	}

	fclose(fp);
}

void 
Component::commit(
		const char* path, 
		char* revision,
		int revsize,
		std::vector<FileInfo>& files)
{
	try {
		::libany::rcpp::RcppDocument cmd(_rcppstream);

		cmd.request_begin_query("component");
		cmd.request_key("name", _name);
		cmd.request_key("brand", _brand);
		cmd.request_key("base", _service);

		cmd.request_begin_data();
			cmd.begin("commit");
				cmd.write_tag("revision", revision);
				cmd.begin("files");
				for(unsigned int i=0; i<files.size(); i++) {
					cmd.begin("file");
					cmd.write_tag("id", files[i].id);
					cmd.write_tag("path", files[i].path);
					cmd.begin("content");
					send_file(path, files[i], cmd);
					cmd.end();
					cmd.end();
				}
				cmd.end();
			cmd.end();
		cmd.request_end();

		if(!cmd.match("/response/revision")) {
			throw std::runtime_error("server didnt send revision");
		}
		int len = cmd.read(revision, revsize);
		if(len <= 0) {
			throw std::runtime_error("server didnt send revision");
		}
		revision[len] = 0;
		
		cmd.match_end();
	}
	catch(...) {
		throw;
	}


}


FileInfo::FileInfo(const char* p_id, const char* p_path)
{
	strcpy(id, p_id);
	strcpy(path, p_path);
}

