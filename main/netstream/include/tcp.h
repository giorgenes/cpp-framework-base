#ifndef __libany__netstream__tcp_h
#define __libany__netstream__tcp_h

#include <libany/stream/stream.h>
#include <libany/io/io.h>

namespace libany {
	namespace netstream {
		class TCPClientIOStream : public ::libany::stream::IOStream {
			private:
				::libany::io::FD _fd;
			public:
				TCPClientIOStream();
				virtual ~TCPClientIOStream();

				int read(void*, int);
				int write(const void*, int);
				bool eos();

				void connect(const char*, int);
		};
	}
}

#endif
