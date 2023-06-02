#include <libany/stdsrvi/ucspi.h>
#include <libany/prodsrv/product.h>

int main()
{
	::libany::productsrv::ProductSrv srv;
	::libany::stdsrvi::Ucspi u(srv);

	u.run();
	
	return 0;
}

