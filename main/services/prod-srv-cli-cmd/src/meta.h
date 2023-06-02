#ifndef __meta_h
#define __meta_h

#include <libany/prodsrvcli/product.h>
#include <vector>

namespace cli = ::libany::prodsrvcli;

class MetaDir {
	private:
		char _path[1024];

		void find();
	public:
		void load_file_list(std::vector< cli::ComponentInfo>*);
		void save_file_list(std::vector< cli::ComponentInfo>&);

		void write_version(const char*);
		void read_version(char*, int);

		void create();

		MetaDir(const char*);
};

#endif
