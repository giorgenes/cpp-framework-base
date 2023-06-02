#ifndef __libany__componentsrvcli__meta_h
#define __libany__componentsrvcli__meta_h

#include <libany/componentsrvcli/component.h>
#include <libany/metadir/md.h>
#include <vector>

namespace libany {
	namespace componentsrvcli {
		class MetaDir : protected ::libany::metadir::MetaDir {
			private:
			public:
				void load_file_list(std::vector< ::libany::componentsrvcli::FileInfo>*);
				void save_file_list(std::vector< ::libany::componentsrvcli::FileInfo>&);

				void write_revision(const char*);
				void read_revision(char*, int);

				void read_name(char*);
				void write_name(const char*);

				MetaDir(const char*);
				void add(const char*);

				::libany::metadir::MetaDir::create;
				::libany::metadir::MetaDir::path;
		};
	}
}
#endif
