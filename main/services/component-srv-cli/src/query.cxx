#include <libany/componentsrvcli/component.h>
#include <stdexcept>
#include <libany/stfactory/uristream.h>
#include <libany/bxtp/document.h>
#include <memory>

using namespace ::libany::componentsrvcli;
				
ComponentQuery::ComponentQuery(::libany::rcpp::RcppStream& r, 
		const char* brand, 
		const char* base)
	: ::libany::rcpp::Client(r), _sax(r)
{
	strcpy(_brand, brand);
	strcpy(_base, base);
	*_name = 0;
}

ComponentQuery::ComponentQuery(::libany::rcpp::RcppStream& r, 
		const char* name,
		const char* brand, 
		const char* base)
	: ::libany::rcpp::Client(r), _sax(r)
{
	strcpy(_name, name);
	strcpy(_brand, brand);
	strcpy(_base, base);
}

void ComponentQuery::list()
{
	try {
		_sax.request_begin_nquery("component");
		_sax.request_key("brand", _brand);
		_sax.request_key("base", _base);
		_sax.request_data("list");
		_sax.request_end();
		_st = 1;
	}
	catch(...) {
		throw;
	}
}

/* FIXME: delete the streams */
bool ComponentQuery::getnext(char* user, int size)
{
	if(!_sax.match("/response/component")) {
		return false;
	}
	
	int len = _sax.read(user, size-1);
	if(len <= 0) {
		return false;
	}
	user[len] = 0;
	
	return true;
}

void ComponentQuery::get_revisions()
{
	_sax.request_begin_nquery("component");
	_sax.request_key("name", _name);
	_sax.request_key("brand", _brand);
	_sax.request_key("base", _base);
	_sax.request_data("listrev");
	_sax.request_end();
	_st = 1;
}

bool 
ComponentQuery::next_revision(char* rev, int revsize, char* childs, int chsize)
{
	if(!_sax.match("/response/revision")) {
		return false;
	}

	_sax.match("/response/revision/number", true);
	int len = _sax.read(rev, revsize-1);
	if(len <= 0) {
		return false;
	}
	rev[len] = 0;
	
	_sax.match("/response/revision/childs", true);
	len = _sax.read(childs, chsize-1);
	if(len <= 0) {
		len = 0;
	}
	childs[len] = 0;

	return true;
}

