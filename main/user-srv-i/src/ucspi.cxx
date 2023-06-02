#include <libany/stdsrvi/ucspi.h>
#include <libany/usersrv/user.h>

int main()
{
	::libany::usersrv::User usersrv;
	::libany::stdsrvi::Ucspi u(usersrv);

	u.run();
	
	return 0;
}

