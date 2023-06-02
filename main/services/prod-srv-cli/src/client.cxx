#include <libany/prodsrvcli/product.h>
#include <stdexcept>
#include <libany/stfactory/uristream.h>
#include <memory>
#include <libany/utils/path_utils.h>

using namespace ::libany::prodsrvcli;

Product::Product(::libany::rcpp::RcppStream& r, 
		const char* name, 
		const char* brand, 
		const char* service)
	: ::libany::rcpp::Client(r)
{
	strcpy(_name, name);
	strcpy(_brand, brand);
	strcpy(_service, service);
}

void Product::create()
{
	try {
		::libany::rcpp::RcppDocument cmd(_rcppstream);
		
		cmd.request_begin_nquery("product");
		cmd.request_key("name", _name);
		cmd.request_key("brand", _brand);
		cmd.request_key("service", _service);
		cmd.request_data("create");
		cmd.request_end();

		cmd.match_end();
	}
	catch(...) {
		throw;
	}
}

void 
Product::checkout(
		const char* version, 
		std::vector<ComponentInfo>* cmpnts)
{
	try {
		::libany::rcpp::RcppDocument cmd(_rcppstream);

		cmd.request_begin_nquery("product");
		cmd.request_key("name", _name);
		cmd.request_key("brand", _brand);
		cmd.request_key("service", _service);

		cmd.request_begin_data();
		cmd.begin("checkout");
		cmd.write_tag("version", version);
		cmd.end();
		cmd.request_end();

		int len;
		while(cmd.match("/response/component")) {
			cmd.match("/response/component/name");
			char name[1024];
			len = cmd.read(name, sizeof(name)-1);
			name[len] = 0;

			cmd.match("/response/component/revision");
			char rev[1024];
			len = cmd.read(rev, sizeof(rev)-1);
			rev[len] = 0;
			
			cmpnts->push_back(ComponentInfo(name, rev));
		}
	}
	catch(...) {
		throw;
	}

}

void 
Product::commit(
		char* version,
		int versize,
		std::vector<ComponentInfo>& components)
{
	try {
		::libany::rcpp::RcppDocument cmd(_rcppstream);

		cmd.request_begin_nquery("product");
		cmd.request_key("name", _name);
		cmd.request_key("brand", _brand);
		cmd.request_key("service", _service);

		cmd.request_begin_data();
			cmd.begin("commit");
				cmd.write_tag("version", version);
				cmd.begin("components");
				for(unsigned int i=0; i<components.size(); i++) {
					cmd.begin("component");
					cmd.write_tag("name", components[i].name);
					cmd.write_tag("revision", components[i].revision);
					cmd.end();
				}
				cmd.end();
			cmd.end();
		cmd.request_end();

		if(!cmd.match("/response/version")) {
			throw std::runtime_error("server didnt send version");
		}
		int len = cmd.read(version, versize);
		if(len <= 0) {
			throw std::runtime_error("server didnt send revision");
		}
		version[len] = 0;
		
		cmd.match_end();
	}
	catch(...) {
		throw;
	}


}


ComponentInfo::ComponentInfo(const char* p_name, const char* p_revision)
{
	strcpy(name, p_name);
	strcpy(revision, p_revision);
}

