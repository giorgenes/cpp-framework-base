#ifndef __libany__stdstream__xmlstream_h
#define __libany__stdstream__xmlstream_h

#include <libany/stream/stream.h>
#include <stdio.h>
#include <expat.h>

namespace libany {
	namespace stdstream {
		class OXmlStream : public virtual ::libany::stream::Stream {
			private:
				XML_Parser parser;
				char current_tag_path[1024];
				int current_tag_path_len;
				int state;
				char* content;
				int content_length;
				int content_length_max;

				static void start(void *, const char *, const char **);
				static void end(void *data, const char *el);
				static void XML_chardata(void *data,const XML_Char *s,int);
			protected:
				virtual void
					tag_process_callback(const char*, const char*) = 0;
				virtual void
					on_tag_begin(const char*, const char**);
				virtual void
					on_tag_end(const char*);

			public:
				int write(const char*, int);
				OXmlStream();
				virtual ~OXmlStream();
		};
	}
}

#endif
