#ifndef __libany__bxtp__document_h
#define __libany__bxtp__document_h

#include <libany/bxtp/stream.h>

namespace libany {
	namespace bxtp {
		struct MatchList {
			const char** items;
			MatchList* next;
		};
		
		class IDocument : public virtual ::libany::stream::IStream {
			private:
				int _tk;
				char _path[2048];
				int _depth;
				bool _end;
				::libany::bxtp::BxtpIStream& _bxtp;
				IDocument* _parent;

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

				IDocument(::libany::bxtp::BxtpIStream &st) 
					: _depth(0), _end(false), _bxtp(st), _parent(0)
				{
					*_path = 0;
				};

				IDocument(IDocument&);

				~IDocument();
		};
		
		class ODocument : public virtual ::libany::stream::OStream {
			private:
				int _wdepth;
				::libany::bxtp::BxtpOStream& _bxtp;
			public:
				void write_tag(const char*, const char* val = 0, int len = 0);
				bool complete_depth(int);

				virtual int write(const void*, int);
				void begin(const char*);
				void end();

				ODocument(::libany::bxtp::BxtpOStream& st) : 
					_wdepth(0),_bxtp(st) {
				}

				~ODocument();
		};

		class IODocument : 
			public IDocument, 
			public ODocument, 
			public virtual ::libany::stream::IOStream 
		{
			protected:
			public:
				IODocument(::libany::bxtp::BxtpIOStream &st) 
					: IDocument(st), ODocument(st) {};

				virtual bool eos();
				virtual int read(void*, int);
				virtual int write(const void*, int);

				virtual ~IODocument();
		};
	}
}

#endif
