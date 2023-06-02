#include <stdio.h>
#include <libany/utils/string_utils.h>
#include <string.h>
#include <libany/stfactory/uristream.h>
#include <libany/bxtp/stream.h>
#include <libany/bxtp/document.h>
#include <stdexcept>

int main(int argc, char** argv)
{
	try {
		::libany::stfactory::URIStream stream("pipe://./server");
		::libany::bxtp::BxtpIOStream bx(stream);

		{
			::libany::bxtp::IODocument doc(bx);
			doc.begin("req");
			doc.write_tag("teste");
			doc.write_tag("foo");
			doc.write_tag("bar", "content");
			doc.end();

			doc.match("/resp");
			doc.match("/resp/uhum");
			doc.match_end();
		}
		
		{
			::libany::bxtp::IODocument doc(bx);
			doc.begin("req");
			doc.write_tag("teste");
			doc.write_tag("foo");
			doc.write_tag("bar", "content");
			doc.end();

			doc.match("/resp");
			doc.match("/resp/uhum");
			doc.match_end();
		}

	}
	catch(std::exception &e) {
		fprintf(stderr, "exception found: %s\n", e.what());
	}
	return 0;
}

