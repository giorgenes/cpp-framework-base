#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <string.h>
#include <libany/stdstream/fd.h>
#include <libany/bxtp/stream.h>
#include <libany/bxtp/document.h>
#include <stdexcept>

int main(int argc, char** argv)
{
	try {
		::libany::stdstream::IOFDStream stream(0, 1);
		::libany::bxtp::BxtpIOStream bx(stream);

		do {
			::libany::bxtp::IODocument doc(bx);

			fprintf(stderr, "esperando /req\n");
			if(!doc.match("/req/teste")) {
				fprintf(stderr, "oops\n");
				break;
			}
			fprintf(stderr, "recebi /req\n");
			
			doc.match("/req/foo");
			doc.match("/req/bar");
			char buf[1024];
			int len = doc.read(buf, sizeof(buf)-1);
			buf[len] = 0;
			fprintf(stderr, "recebi: %s\n", buf);
			doc.match_end();

			doc.begin("resp");
			doc.write_tag("uhum");
			doc.end();
		}while(1);
		fprintf(stderr, "fim na boa\n");
	}
	catch(std::exception &e) {
		fprintf(stderr, "exception found: %s\n", e.what());
	}
	return 0;
}

