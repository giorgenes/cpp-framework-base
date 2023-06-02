#include <libany/stdsrvi/ucspi.h>
#include <libany/netstream/fd.h>
#include <stdexcept>

using namespace ::libany::stdsrvi;

void Ucspi::run()
{	
	try {
		::libany::netstream::FDStream fd(0, 1);

		Ucspi::service.handle_client(&fd);
	}
	catch(std::exception &e) {
		fprintf(stderr, "exception: %s\n", e.what());
	}
}

