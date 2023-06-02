#ifndef __libany__strfile__strfile_h
#define __libany__strfile__strfile_h

#include <cstdio>

namespace libany {
	namespace strfile {
		typedef struct {
			unsigned int n;
			unsigned int idxb;
			unsigned int keyb;
			unsigned int datab;
		}header_t;
		
		class StrFile {
			private:
				char* __str;
				std::FILE* __fp;
				header_t __h;
				unsigned int *__keyidx;
				unsigned int *__dataidx;
			public:
				inline const char* str() const;
				void loadstr(const char*);
				void load(const char*);

				StrFile();
		};

		inline const char* StrFile::str() const
		{
			return __str;
		}
	}
}

#endif
