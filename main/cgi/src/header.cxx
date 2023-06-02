#include "cgi.h"
#include <string.h>
#include <stdio.h>

using namespace ::libany::cgi;

Header::Header(CGIEnv& env)
	: _env(env), _wrote_content(false)
{
}

void Header::write_content_type(const char* s)
{
	int len = strlen(s);

	_env.io.write("Content-type: ", 14);
	_env.io.write(s, len);
	_env.io.write("\n", 1);

	_wrote_content = true;
}

void Header::complete()
{
	if(!_wrote_content) {
		write_content_type("text/plain");
	}
	_env.io.write("\n", 1);
}

void Header::write_cookie(
		const char* key, 
		const char* value, 
		const char* domain, 
		const char* path,
		time_t expires,
		bool secure)
{
	int len;

	/* ref: http://wp.netscape.com/newsref/std/cookie_spec.html */
	_env.io.write("Set-Cookie: ", 12);

	/* NAME=VALUE; */
	len = strlen(key);
	_env.io.write(key, len);
	_env.io.write("=", 1);
	len = strlen(value);
	_env.io.write(value, len);

	char buf[2048];
#if 0
	/* expires=DATE; */
	_env.io.write("; expires=", 10);
	struct tm ttm;
	gmtime_r(&expires, &ttm);
	strftime(buf, sizeof(buf), "%z", &ttm);
	len = strlen(buf);
	_env.io.write(buf, len);
#endif

	/* path=PATH; domain=DOMAIN_NAME; secure */
	len = snprintf(buf, sizeof(buf), "; path=%s; domain=%s;%s\n",
			path, 
			domain,
			secure ? " secure" : "");
	_env.io.write(buf, len);
}

