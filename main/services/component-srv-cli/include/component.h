#ifndef __libany__componentsrvcli__component_h
#define __libany__componentsrvcli__component_h

#include <libany/ios/ios.h>
#include <libany/stfactory/uristream.h>
#include <libany/rcpp/service.h>
#include <vector>

namespace libany {
	namespace componentsrvcli {
		class ComponentQuery : protected ::libany::rcpp::Client {
			private:
				char _name[128];
				char _brand[128];
				char _base[128];
				int _st;
				::libany::rcpp::RcppDocument _sax;
			public:
				void list();
				bool getnext(char*, int);
				
				void get_revisions();
				bool next_revision(char*, int, char*, int);

				ComponentQuery(::libany::rcpp::RcppStream& r, 
						const char*, const char*);
				
				ComponentQuery(::libany::rcpp::RcppStream& r, 
						const char*, const char*, const char*);

		};

		class FileInfo {
			public:
				FileInfo(const char*, const char*);

				char id[128];
				char path[1024];
		};

		class Component : protected ::libany::rcpp::Client {
			private:
				char _name[128];
				char _brand[128];
				char _service[128];
			public:
				Component(::libany::rcpp::RcppStream&, 
						const char*, const char*, const char*);

				void create();
				void checkout(const char*, const char*, std::vector<FileInfo>*);
				void commit(const char*, char*, int, std::vector<FileInfo>&);
		};
	}
}

#endif
