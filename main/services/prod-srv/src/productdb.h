#ifndef __productdb_h
#define __productdb_h

#include <string.h>
#include "product.h"
#include <stdio.h>

class Product;
class ProductDB {
	private:
		FILE* _fp;
	public:
		char base[128];
		char brand[128];

		ProductDB(const char* _brand, const char* _base)
			: _fp(0) {
			strcpy(brand, _brand);
			strcpy(base, _base);
		};

		void first();
		bool next(Product&);

		~ProductDB();
};

#endif
