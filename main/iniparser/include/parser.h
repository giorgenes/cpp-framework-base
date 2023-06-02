#ifndef __libany__iniparser__parser_h
#define __libany__iniparser__parser_h

#include <libany/stream/stream.h>
#include <libany/stdstream/bufstream.h>

namespace libany {
	namespace iniparser {
		class Parser {
			private:
				::libany::stdstream::IBufStream _st;
				char _last_session[128];
				int _line;
			protected:
				virtual void on_conf(const char*, const char*, const char*);
			public:
				Parser(::libany::stream::IStream& st)
					: _st(st), _line(0) {
						*_last_session = 0;
				};

				bool next_conf(char*, int, char*, int, char*, int);
				void parse();

				virtual ~Parser();
		};
	}
}

#endif
