#ifndef __libany__stdstream__complete_h
#define __libany__stdstream__complete_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class CompleteStream : public virtual ::libany::stream::Stream {
			private:
				::libany::stream::Stream& _st;
			public:
				CompleteStream(::libany::stream::Stream &st) 
					: _st(st) {
				};

				virtual int read(char*, int);
				virtual int write(const char*, int);
				virtual bool eos();
				virtual void close();
		};

	}
}

#endif
