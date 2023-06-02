#ifndef __libany__netstream__fd_h
#define __libany__netstream__fd_h

#include <libany/stream/stream.h>
#include <libany/io/io.h>

namespace libany {
	namespace netstream {
		class FDStream : public ::libany::stream::Stream {
			private:
				::libany::io::FD fd;
			public:
				FDStream(int rfd, int wfd) :
					fd(rfd, wfd) {};
				virtual ~FDStream();

				int read(void*, int);
				int write(const void*, int);
				bool eos();
		};
	}
}

#endif
