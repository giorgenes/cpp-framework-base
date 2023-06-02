#include "opt.h"
#include <libany/iniparser/parser.h>
#include <libany/stdstream/filestream.h>
#include <string.h>

void Opt::parse()
{
	try {
		::libany::stdstream::IFileStream file("./config.ini");
		::libany::iniparser::Parser parser(file);

		char s[1024];
		char k[1024];
		char v[1024];
		while(parser.next_conf(s, sizeof(s), k, sizeof(k), v, sizeof(v))) {
			if(strcmp(s, "usersrv")==0) {
				if(strcmp(k, "base_uri")==0) {
					strcpy(base, v);
				}
			}
			else if(strcmp(s, "commom")==0) {
				if(strcmp(k, "brand")==0) {
					strcpy(brand, v);
				}

			}
		}
	}
	catch(...) {
		throw;
	}
}

