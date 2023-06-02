#ifndef __libany__prodsrvcli__product_h
#define __libany__prodsrvcli__product_h

#include <libany/ios/ios.h>
#include <libany/stfactory/uristream.h>
#include <libany/rcpp/service.h>
#include <vector>

namespace libany {
	namespace prodsrvcli {
		class ProductQuery : protected ::libany::rcpp::Client {
			private:
				char _name[128];
				char _brand[128];
				char _base[128];
				int _st;
				::libany::rcpp::RcppDocument _sax;
			public:
				void list();
				bool getnext(char*, int);
				
				void get_versions();
				bool next_version(char*, int, char*, int);

				ProductQuery(::libany::rcpp::RcppStream& r, 
						const char*, const char*);
				
				ProductQuery(::libany::rcpp::RcppStream& r, 
						const char*, const char*, const char*);

		};

		class ComponentInfo {
			public:
				ComponentInfo(const char*, const char*);

				char name[128];
				char revision[64];
		};

		class Product : protected ::libany::rcpp::Client {
			private:
				char _name[128];
				char _brand[128];
				char _service[128];
			public:
				Product(::libany::rcpp::RcppStream&, 
						const char*, const char*, const char*);

				void create();
				void checkout(const char*, std::vector<ComponentInfo>*);
				void commit(char*, int, std::vector<ComponentInfo>&);
		};
	}
}

#endif
