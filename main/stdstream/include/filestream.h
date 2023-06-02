#ifndef __libany__stdstream__filestream_h
#define __libany__stdstream__filestream_h

#include <libany/stream/stream.h>
#include <stdio.h>

namespace libany {
	namespace stdstream {
		class IFileStream : public ::libany::stream::IStream {
			private:
				FILE* _fp;
				bool _owner;
			public:
				IFileStream(const char*);
				IFileStream(FILE*);
				virtual ~IFileStream();

				int read(void*, int);
				bool eos();
		};
		
		class OFileStream : public ::libany::stream::OStream {
			private:
				FILE* _fp;
				bool _owner;
			public:
				OFileStream(const char*);
				OFileStream(FILE*);
				virtual ~OFileStream();

				int write(const void*, int);
		};

	}
}

#endif
