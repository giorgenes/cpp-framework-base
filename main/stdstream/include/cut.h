#ifndef __libany__stdstream__cut_h
#define __libany__stdstream__cut_h

#include <libany/stream/stream.h>
#include <stdio.h>

namespace libany {
	namespace stdstream {
		class ICutStream : public ::libany::stream::IStream {
			private:
				::libany::stream::IStream &_st;
				unsigned int _max;
				unsigned int _count;
			public:
				ICutStream(::libany::stream::IStream &st, int max)
					: _st(st), _max(max), _count(0)
				{
				};
				
				ICutStream(::libany::stream::IStream &st)
					: _st(st), _max(0), _count(0)
				{
				};

				inline void set_limit(int max) {
					_max = max;
				};

				int read(void*, int);
				bool eos();
		};
	}
}

#endif
