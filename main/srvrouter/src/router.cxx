#include <libany/srvrouter/router.h>

using namespace ::libany::srvrouter;

const char* Router::route(const char* service, const char* key)
{
	return "tcp://localhost:12121";
}

void Router::configure(const char* key, const char* value)
{
}

