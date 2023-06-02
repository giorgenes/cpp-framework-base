#ifndef __libany__stream_h
#define __libany__stream_h

namespace libany {
	namespace stream {
		class OStream;
		class IStream {
			public:
				virtual int read(void*, int) = 0;
				virtual bool eos() = 0;

				virtual int copyto(OStream*);
		};

		class OStream {
			public:
				virtual int write(const void*, int) = 0;

				virtual int copyfrom(IStream*);
		};

		class IOStream : public IStream, public OStream {
		};

		class BlockStream : public IStream, public OStream {
		};
	}
}

#endif
