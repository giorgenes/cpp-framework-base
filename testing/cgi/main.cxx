#include <libany/cgi/cgi.h>
#include <libany/cgi/stdcgi.h>
#include <libany/stdstream/urldecode.h>
#include <stdio.h>

namespace cgi = ::libany::cgi;
namespace stream {
	using namespace ::libany::stdstream;
}

class MyCGI : public cgi::CGIApp {
	public:
		void write_header(const cgi::CGIEnv&, cgi::Header&);
		void write_body(const cgi::CGIEnv&);
};

void MyCGI::write_header(
		const cgi::CGIEnv& env, cgi::Header& header)
{
	header.write_content_type("text/plain");
	header.write_cookie(
			"SID", 
			"892173ghjkhas01928sdf", 
			"trinita.canopus", 
			"/", 
			time(NULL)+1000, 
			false);
	
	header.write_cookie("colosso", 
			"test", "trinita.canopus", "/", time(NULL)+1000, false);
}

void MyCGI::write_body(const cgi::CGIEnv& env)
{
	int len;
	char buf[1024];
	stream::IUrlDecodeStream in(env.io);

	len = sprintf(buf, "query_string: %s\n", env.query_string);
	env.io.write(buf, len);

	len = sprintf(buf, "path_info: %s\n", env.path_info);
	env.io.write(buf, len);

	len = sprintf(buf, "cookies: %s\n", env.cookies);
	env.io.write(buf, len);

	while(in.get_next_pair()) {
		len = sprintf(buf, "key: %s\ndata: ", in.key());
		env.io.write(buf, len);

		while((len = in.read(buf, sizeof(buf))) > 0) {
			env.io.write(buf, len);
		}
		env.io.write("\n", 1);
	}
}

int main(int argc, char** argv)
{
	MyCGI c;
	cgi::StdCGI cgi(c);

	cgi.run(argc, argv);

	return 0;
}

