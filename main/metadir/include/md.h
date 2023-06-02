#ifndef __libany__metadir__md_h
#define __libany__metadir__md_h

namespace libany {
	namespace metadir {
		class MetaDir {
			private:
				char _name[32];
				char _path[1024];

				void find(char*);
			public:
				void read_single_value(const char*, char*);
				void write_single_value(const char*, const char*);

				void get_key_path(const char*, char*);

				MetaDir(const char*, const char*);

				inline const char* path() {
					return _path;
				};

				void create();
		};
	}
}


#endif
