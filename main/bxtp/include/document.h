#ifndef __libany__bxtp__document_h
#define __libany__bxtp__document_h

#include <libany/bxtp/stream.h>

namespace libany {
	namespace bxtp {
		struct MatchList {
			const char** items;
			MatchList* next;
		};
		
		class Document : public virtual ::libany::stream::Stream {
			private:
				int _tk;
				char _path[2048];
				int _depth;
				bool _end;
				::libany::bxtp::BxtpStream& _bxtp;
				Document* _parent;
				int _wdepth;

				void gotodata();
				bool next_token(int*);
				bool match_next();

				void path_cat();
				void path_cut();

				void reset();
			public:
				virtual bool match(
						MatchList*, int*, int*, bool throw_exception = false);
				virtual bool match(
						const char**, int*, bool throw_exception = false);
				virtual bool match(const char*, bool throw_exception = false);
				virtual void match_end();
				
				virtual bool eos();
				virtual int read(void*, int);

				Document(::libany::bxtp::BxtpStream &st) 
					: _depth(0), _end(false), _bxtp(st), _parent(0), _wdepth(0)
				{
					*_path = 0;
				};

				Document(Document&);

				~Document();
		
				void write_tag(const char*, const char* val = 0, int len = 0);
				bool complete_depth(int);

				virtual int write(const void*, int);
				void begin(const char*);
				void end();
		};

	}
}

#endif
