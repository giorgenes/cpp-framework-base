#include <libany/srvrouter/router.h>

using namespace ::libany::srvrouter;

/* FIXME: make it fast */
const char* Router::route(const char* service, const char* key)
{
	unsigned int i;
	for(i=0; i<_services.size(); i++) {
		if(strcmp(_services[i].name, service)==0) {
			return _services[i].address;
		}
	}

	return 0;
}

void Router::configure(const char* key, const char* value)
{
	if(value != 0) {
		if(strcmp(key, "/config/services/service/name")==0) {
			strcpy(_auxitem.name, value);
		}
		else if(strcmp(key, "/config/services/service/address")==0) {
			strcpy(_auxitem.address, value);
		}
	}
	else {
		if(strcmp(key, "/config/services/service")==0) {
			_services.push_back(_auxitem);
		}
	}
}

void Router::search(const char* srv, const char* key)
{
	unsigned int i;
	for(i=0; i<_services.size(); i++) {
		if(strcmp(_services[i].name, srv)==0) {
			_idx = i;
			break;
		}
	}
}

const char* Router::next()
{
	if(_idx >= 0) {
		const char* p = _services[_idx].address;
		_idx = -1;
		return p;
	}
	return 0;
}


Router::~Router()
{
}

