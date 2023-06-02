#include <libany/cgi/stdcgi.h>
#include <stdio.h>

class TestCGI : public ::libany::cgi::CGI {
	public:
		void write_header(const ::libany::cgi::CGIEnv&);
		void write_body(const ::libany::cgi::CGIEnv&);
};

void TestCGI::write_header(const ::libany::cgi::CGIEnv& env)
{
	fprintf(stdout, "Content-Type: text/plain\n\n");
}

void TestCGI::write_body(const ::libany::cgi::CGIEnv& env)
{
	fprintf(stdout, "foo\n");
}

int main(int argc, char** argv)
{
	TestCGI cgi;
	::libany::cgi::StdCGI s(cgi);

	s.run(argc, argv);
	
	return 0;
}

