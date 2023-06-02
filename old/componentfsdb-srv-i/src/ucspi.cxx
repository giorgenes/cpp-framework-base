#include <libany/stdsrvi/ucspi.h>
#include <libany/componentfsdbsrv/srv.h>

int main()
try
{
	::libany::componentfsdbsrv::ComponentDBSrv srv;
	::libany::stdsrvi::Ucspi u(srv);

	u.run();
	
	return 0;
}
catch(...) {
}

