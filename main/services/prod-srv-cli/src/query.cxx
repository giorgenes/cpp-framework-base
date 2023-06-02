#include <libany/prodsrvcli/product.h>
#include <stdexcept>
#include <libany/stfactory/uristream.h>
#include <libany/bxtp/document.h>
#include <memory>

using namespace ::libany::prodsrvcli;
				
ProductQuery::ProductQuery(::libany::rcpp::RcppStream& r, 
		const char* brand, 
		const char* base)
	: ::libany::rcpp::Client(r), _sax(r)
{
	strcpy(_brand, brand);
	strcpy(_base, base);
	*_name = 0;
}

ProductQuery::ProductQuery(::libany::rcpp::RcppStream& r, 
		const char* name,
		const char* brand, 
		const char* base)
	: ::libany::rcpp::Client(r), _sax(r)
{
	strcpy(_name, name);
	strcpy(_brand, brand);
	strcpy(_base, base);
}

void ProductQuery::list()
{
	try {
		_sax.request_begin_nquery("product");
		_sax.request_key("brand", _brand);
		_sax.request_key("service", _base);
		_sax.request_data("list");
		_sax.request_end();
		_st = 1;
	}
	catch(...) {
		throw;
	}
}

/* FIXME: delete the streams */
bool ProductQuery::getnext(char* prod, int size)
{
	if(!_sax.match("/response/product")) {
		return false;
	}
	
	int len = _sax.read(prod, size-1);
	if(len <= 0) {
		return false;
	}
	prod[len] = 0;
	
	return true;
}

void ProductQuery::get_versions()
{
	_sax.request_begin_nquery("product");
	_sax.request_key("name", _name);
	_sax.request_key("brand", _brand);
	_sax.request_key("service", _base);
	_sax.request_data("listver");
	_sax.request_end();
	_st = 1;
}

bool 
ProductQuery::next_version(char* rev, int revsize, char* childs, int chsize)
{
	if(!_sax.match("/response/version")) {
		return false;
	}

	_sax.match("/response/version/number", true);
	int len = _sax.read(rev, revsize-1);
	if(len <= 0) {
		return false;
	}
	rev[len] = 0;
	
	_sax.match("/response/version/childs", true);
	len = _sax.read(childs, chsize-1);
	if(len <= 0) {
		len = 0;
	}
	childs[len] = 0;

	return true;
}

