#ifndef __libany__stdstream__filestream_h
#define __libany__stdstream__filestream_h

#include <libany/stream/stream.h>
#include <stdio.h>

namespace libany {
	namespace stdstream {
		class IOFileStream : public virtual ::libany::stream::Stream {
			private:
				FILE* _fp;
				bool _owner;
			public:
				IOFileStream(const char*);
				IOFileStream(const char*, const char*);
				IOFileStream(FILE*);
				virtual ~IOFileStream();

				int read(char*, int);
				bool eos();
				int write(const char*, int);
		};

	}
}

#endif
