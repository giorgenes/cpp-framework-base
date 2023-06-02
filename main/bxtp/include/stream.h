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
		class BxtpStream : public virtual ::libany::stream::Stream {
			private:
				::libany::stream::Stream& _ou;
				::libany::stdstream::CompleteStream _complete_stream;
				::libany::stdstream::BufStream _in;
				
				int _st;
				char _tag[256+1];
			public:
				void begin(const char*);
				void end();
				void reset();
				
				BxtpStream(::libany::stream::Stream &st) 
					:
					_ou(st),
					_complete_stream(st), 
					_in(_complete_stream), 
				  _st(0) {};

				bool next_token(int* token);
				inline const char* tag() { return _tag; };
				
				virtual int read(void*, int);
				virtual int write(const void*, int);
				virtual bool eos();

				~BxtpStream();
		};
	}
}

#endif
