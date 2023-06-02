#ifndef __libany__stdstream__fd_h
#define __libany__stdstream__fd_h

#include <libany/stream/stream.h>
#include <libany/io/io.h>

namespace libany {
	namespace stdstream {
		class IOFDStream : public virtual ::libany::stream::Stream {
			private:
				int _in;
				int _ou;
				::libany::io::FD _fd;
			protected:
				void setfd(int, int);
			public:
				IOFDStream(int, int);
				int read(char*, int);
				bool eos();
				int write(const char*, int);

				virtual ~IOFDStream() {
					IOFDStream::close();
				};

				virtual void close();
		};
	}
}

#endif
