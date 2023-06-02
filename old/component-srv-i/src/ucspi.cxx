#include <libany/stdsrvi/ucspi.h>
#include <libany/componentsrv/component.h>

int main()
try
{
	::libany::componentsrv::ComponentSrv componentsrv;
	::libany::stdsrvi::Ucspi u(componentsrv);

	u.run();
	
	return 0;
}
catch(...) {
}

