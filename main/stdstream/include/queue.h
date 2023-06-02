#ifndef __libany__stdstream__queue_h
#define __libany__stdstream__queue_h

#include <libany/stream/stream.h>

namespace libany {
	namespace stdstream {
		class QueueStream : public virtual ::libany::stream::Stream {
			private:
				char* _buf;
				int _max;
				int _b;
				int _e;

				void alloc_enougth_room(int);
			public:
				QueueStream()
					: _buf(0), _max(0), _b(0), _e(0) {}

				int read(char*, int);
				bool eos();
				int write(const char*, int);
				inline int size() const;
		};

		int QueueStream::size() const
		{
			if(_e >= _b) {
				return _e - _b;
			}
			else {
				return (_max - _b) + _e;
			}
		}
	}
}

#endif
