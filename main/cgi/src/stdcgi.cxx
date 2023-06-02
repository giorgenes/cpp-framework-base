#include "stdcgi.h"
#include <stdlib.h>
#include <string.h>
#include <libany/stdstream/composite.h>
#include <libany/stdstream/enable.h>
#include <libany/stdstream/filestream.h>
#include <libany/stdstream/cut.h>

using namespace ::libany::cgi;
namespace stm = ::libany::stdstream;

static void get_cgi_env(::libany::cgi::CGIEnv& env)
{
	const char* p;

	p = getenv("REQUEST_METHOD");
	if(!p) return;

	if(strcmp(p, "POST")==0) {
		env.method = post;
	}
	else if(strcmp(p, "HEAD")==0) {
		env.method = head;
	}
	else if(strcmp(p, "GET")==0) {
		env.method = get;
	}

	env.content_len = 0;
	p = getenv("CONTENT_LENGTH");
	if(p) {
		env.content_len = atoi(p);
	}
	
	env.query_string = getenv("QUERY_STRING");
	env.path_info = getenv("PATH_INFO");
	env.cookies = getenv("HTTP_COOKIE");
}

void StdCGI::run(int argc, char**argv)
{
	stm::OFileStream stout(stdout);
	stm::IFileStream stin(stdin);
	stm::IEnableStream en(stin);;
	stm::ICutStream cut(en);
	stm::IOCompositeStream s(cut, stout);
	CGIEnv env(s);
	
	get_cgi_env(env);

	if(env.method == post) {
		en.set_enable(true);
	}
	/* limit the input stream to content_len bytes */
	cut.set_limit(env.content_len);

	{
		Header header(env);
		_app.write_header(env, header);
		header.complete();
	}
	_app.write_body(env);

	/* reads the remaining data, 
	 * to avoid delays in the browser */
	{
		char buf[1024];
		while(env.io.read(buf, sizeof(buf))>0) {
		}
	}
}

