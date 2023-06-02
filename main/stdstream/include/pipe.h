#ifndef __libany__stdstream__pipe_h
#define __libany__stdstream__pipe_h

#include <libany/stdstream/fd.h>

namespace libany {
	namespace stdstream {
		class IOPipeStream : public virtual ::libany::stdstream::IOFDStream {
			private:
				int _pid;
			public:
				IOPipeStream(const char*);

				virtual void close();
				virtual ~IOPipeStream();
		};
	}
}

#endif
