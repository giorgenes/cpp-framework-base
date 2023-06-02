#ifndef __libany__bxtp__stream_h
#define __libany__bxtp__stream_h

#include <libany/stream/stream.h>
#include <libany/stdstream/complete.h>
#include <libany/stdstream/bufstream.h>

#define LIBANY_BXTP_TOKEN_BEGIN   0
#define LIBANY_BXTP_TOKEN_DATA    1
#define LIBANY_BXTP_TOKEN_END     2
#define LIBANY_BXTP_TOKEN_RESET   4

namespace libany {
	namespace bxtp {
		class BxtpOStream : public virtual ::libany::stream::OStream {
			private:
				::libany::stream::OStream& _ou;
			protected:
			public:
				BxtpOStream(::libany::stream::OStream &st)
					: _ou(st)
				  {};

				virtual ~BxtpOStream();

				virtual int write(const void*, int);
				void begin(const char*);
				void end();
				void reset();
		};

		class BxtpIStream : public virtual ::libany::stream::IStream 
		{
			private:
				::libany::stdstream::ICompleteStream _complete_stream;
				::libany::stdstream::IBufStream _in;
				
				int _st;
				char _tag[256+1];
			public:
				BxtpIStream(::libany::stream::IStream &st) 
					:
					_complete_stream(st), 
					_in(_complete_stream), 
				  _st(0) {};

				virtual int read(void*, int);
				virtual bool eos();
				bool next_token(int* token);
				inline const char* tag() { return _tag; };
				
				virtual ~BxtpIStream();
		};
		
		class BxtpIOStream 
			: 
			public BxtpOStream,
			public BxtpIStream,
			public virtual ::libany::stream::IOStream
		{
			private:
			public:
				BxtpIOStream(::libany::stream::IOStream &st) 
					: BxtpOStream(st), BxtpIStream(st) {};

				virtual ~BxtpIOStream();

				virtual int read(void*, int);
				virtual int write(const void*, int);
				virtual bool eos();
		};
	}
}

#endif
