#include <libany/rcpp/session.h>
#include <stdexcept>
#include <string.h>
	
using namespace ::libany::rcpp;

void RcppDocument::request_key(const char* key, const char* value, int len)
{
	write_tag(key, value, len);
}

void RcppDocument::request_data(const char* key, const char* value, int len)
{
	request_begin_data();
	write_tag(key, value, len);
}

void 
RcppDocument::request_begin_nquery(const char* service, const char* base)
{
	_keyclosed = false;
	_dataclosed = false;
	
	begin("request");
	write_tag("nquery");
	write_tag("service", service);
	write_tag("base", base);
	begin("key");
}

void 
RcppDocument::request_begin_query(const char* service, const char* base)
{
	_keyclosed = false;
	_dataclosed = false;
	
	begin("request");
	write_tag("query");
	write_tag("service", service);
	write_tag("base", base);
	begin("key");
}


void RcppDocument::request_begin_data()
{
	if(!_keyclosed) {
		end();
		_keyclosed = true;
		begin("data");
	}
}

void RcppDocument::request_end()
{
	if(!_keyclosed) {
		end();
	}
	
	if(!_dataclosed) {
		end();
	}

	end();
}

#define EXCEPT_PATH "/response/exception"

void RcppDocument::throw_exception()
{
	char buf[1024];
	int len = RcppDocument::read(buf, sizeof(buf)-1);
	if(len <= 0) {
		sprintf(buf, "could not get exception");
	}
	else {
		buf[len] = 0;
	}
	throw std::runtime_error(buf);
}

bool RcppDocument::match(const char** tags, int* idx, bool throwe)
{
	::libany::bxtp::MatchList list[2];
	const char* items[] = {
		EXCEPT_PATH,
		0
	};

	list[0].items = items;
	list[0].next = &list[1];

	list[1].items = tags;
	list[1].next  = 0;

	int l;
	bool m = Document::match(list, &l, idx, throwe);

	if(m && l == 0 && *idx == 0) {
		throw_exception();
	}
	return m;
}

bool RcppDocument::match(const char* tag, bool throwe)
try
{
	const char* tags[] = {
		EXCEPT_PATH,
		tag,
		0
	};
	
	int idx;
	bool m = Document::match(tags, &idx, throwe);
	if(m && idx == 0) {
		throw_exception();
	}

	return m;
}
catch(...) {
	throw;
}

void RcppDocument::match_end()
{
	if(Document::match(EXCEPT_PATH)) {
		throw_exception();
	}
}

