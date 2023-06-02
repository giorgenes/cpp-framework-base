#include <libany/stdsrvi/ucspi.h>
#include <libany/postgresrv/srv.h>

int main()
{
	::libany::postgresrv::PostgreSrv srv;
	::libany::stdsrvi::Ucspi u(srv);

	u.run();
	
	return 0;
}

