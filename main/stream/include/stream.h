#ifndef __libany__stream_h
#define __libany__stream_h

namespace libany {
	namespace stream {
		class Stream {
			public:
				virtual int read(char*, int) = 0;
				virtual int write(const char*, int) = 0;
				virtual bool eos() = 0;

				virtual int copyto(Stream*);
				virtual int copyfrom(Stream*);

				virtual void close();

				virtual ~Stream();
		};
	}
}

#endif
