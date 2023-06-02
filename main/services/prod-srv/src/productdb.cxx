#include "productdb.h"
#include <stdexcept>
#include <libany/utils/string_utils.h>
		
void ProductDB::first()
{
	if(!_fp) {
		char buf[1024];

		sprintf(buf, "/tmp/productsrvdb/%s.%s.txt", brand, base);
		_fp = fopen(buf, "r");
		if(!_fp) {
			throw std::runtime_error("cant open productsrvdb");
		}
	}
}

bool ProductDB::next(Product& product)
{
	char buf[1024];

	if(!_fp) return false;
	
	if(fgets(buf, sizeof(buf), _fp)==NULL) {
		return false;
	}

	char* p = ::libany::utils::string_trimall(buf, 0);
	strcpy(product.name, p);

	return true;
}

ProductDB::~ProductDB()
{
	if(_fp) {
		fclose(_fp);
	}
}

