#include <stdio.h>
#include "vta.h"

static void create_tables(void)
{
	const char* key[100];
	const char* dat[100];
	int c;
	int db;
	
	//db = vta_create_database("default");
	
	c = 0;
	key[c++] = "id"; key[c++] = "i";
	key[c] = NULL;

	c = 0;
	dat[c++] = "name"; dat[c++] = "s25";
	dat[c++] = "age";  dat[c++] = "i";
	dat[c] = NULL;
	
	//vta_create_table("users", key, dat, db);
}

int main()
{
	const char* key[100];
	const char* data[100];

	vta_init();

	create_tables();
	
	vta_begin();
	key[0] = "fulano";
	
	vta_get("table_test", key, data);

	printf("dado: %s\n", data[0]);
	vta_end();

	vta_begin();
	vta_end();

	vta_finish();
}
