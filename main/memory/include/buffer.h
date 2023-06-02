#ifndef __libany__memory__buffer_h
#define __libany__memory__buffer_h

namespace libany {
	namespace memory {
		class Buffer {
			private:
				char* _buf;
				int _size;
				int _max;
			public:
				operator const void* () const {
					return _buf;
				};
				
				operator const char*() const {
					return _buf;
				};

				operator char*() {
					return _buf;
				};

				inline unsigned int size() const {
					return _size;
				};
				
				void set_size(int);
				void set_capacity(int);
				void grow(int);
				void grow_capacity(int);

				Buffer() : 
					_buf(0), _size(0), _max(0) {};

				~Buffer();
		};
	}
}

#endif
