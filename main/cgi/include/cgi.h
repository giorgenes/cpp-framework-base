#ifndef __libany__cgi__cgi_h
#define __libany__cgi__cgi_h

#include <libany/stream/stream.h>
#include <time.h>

namespace libany {
	namespace cgi {
		enum CGIMethod {
			post, get, put, head
		};
		class CGIEnv {
			public:
				unsigned int content_len;
				CGIMethod method;
				const char* query_string;
				const char* path_info;
				const char* cookies;

				/* in oposition to what the
				 * CGI specification says,
				 * in input stream MUST have
				 * an EOF, so the caller must
				 * provide a stream that has
				 * and end. It free's the CGI
				 * of the need to controll the
				 * CONTENT_LENGTH var. */
				::libany::stream::Stream& io;

				CGIEnv(::libany::stream::Stream& _io)
					: io(_io) {
				};
		};

		class Header {
			private:
				CGIEnv& _env;
				bool _wrote_content;
			public:
				Header(CGIEnv& env);
				void write_content_type(const char*);
				void write_cookie(const char* key, 
						const char* value, 
						const char* domain, 
						const char* path,
						time_t expires,
						bool secure);
				void complete();
		};
		
		class CGIApp {
			protected:
			public:
				virtual void write_header(const CGIEnv&, Header&) = 0;
				virtual void write_body(const CGIEnv&) = 0;

				virtual ~CGIApp();
		};
	}
}

#endif
