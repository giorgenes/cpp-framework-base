#ifndef __libany__estp__stream_h
#define __libany__estp__stream_h

#include <libany/stream/stream.h>
#include <libany/stdstream/complete.h>
#include <libany/stdstream/bufstream.h>
#include <libany/stdstream/queue.h>

namespace libany {
	namespace estp {
		class ObjectStream : public virtual ::libany::stream::Stream {
			private:
				::libany::stream::Stream& _ou;
				::libany::stdstream::CompleteStream _complete_stream;
				::libany::stdstream::BufStream _in;
				::libany::stdstream::QueueStream _queue;
				
				int _st;
				char _tag[256+1];

				void init_read();
				void init_write();
			protected:
				short _guid;
			public:
				void begin(const char*);
				void end();
				void reset();
				
				ObjectStream(::libany::stream::Stream &st) 
					:
					_ou(st),
					_complete_stream(st), 
					_in(_complete_stream), 
				  _st(0), _guid(0) {};

				virtual int read(char*, int);
				virtual int write(const char*, int);
				virtual bool eos();
				virtual void close();

				~ObjectStream();
		};
	}
}

#endif
