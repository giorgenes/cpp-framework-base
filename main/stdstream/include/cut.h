#ifndef __libany__stdstream__cut_h
#define __libany__stdstream__cut_h

#include <libany/stream/stream.h>
#include <stdio.h>

namespace libany {
	namespace stdstream {
		class CutStream : public virtual ::libany::stream::Stream {
			private:
				::libany::stream::Stream &_st;
				unsigned int _max;
				unsigned int _count;
			public:
				CutStream(::libany::stream::Stream &st, int max)
					: _st(st), _max(max), _count(0)
				{
				};
				
				CutStream(::libany::stream::Stream &st)
					: _st(st), _max(0), _count(0)
				{
				};

				inline void set_limit(int max) {
					_max = max;
				};

				int read(char*, int);
				bool eos();
		};
	}
}

#endif
