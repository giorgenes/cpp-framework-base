#include "opt.h"
#include <stdexcept>
#include <libany/iniparser/parser.h>
#include <libany/stdstream/filestream.h>

OptParser::OptParser()
	: command(COMMAND_NONE)
{
	strcpy(service, "default");
		*base_uri = 0;
}

void OptParser::show_help(void)
{
	fprintf(stderr, "show some help\n");
}

void OptParser::parse(int argc,  char** argv)
{
	if(argc < 2) {
		show_help();
		throw std::runtime_error("invalid args");
	}

	if(strcmp(argv[1], "commit")==0) {
		if(argc < 3) {
			throw std::runtime_error("invalid args");
		}
		strcpy(name, argv[2]);
		command = COMMAND_COMMIT;
	}
	else if(strcmp(argv[1], "create")==0) {
		if(argc < 3) {
			throw std::runtime_error("invalid args");
		}
		command = COMMAND_CREATE;
		strcpy(name, argv[2]);
	}
	else if(strcmp(argv[1], "listver")==0) {
		if(argc < 3) {
			throw std::runtime_error("invalid args");
		}
		command = COMMAND_LISTREV;
		strcpy(name, argv[2]);
	}
	else if(strcmp(argv[1], "get")==0) {
	}
	else if(strcmp(argv[1], "checkout")==0) {
		if(argc < 4) {
			throw std::runtime_error("invalid args");
		}
		command = COMMAND_CHECKOUT;
		strcpy(name, argv[2]);
		strcpy(version, argv[3]);
	}
	else if(strcmp(argv[1], "merge")==0) {
	}
	else if(strcmp(argv[1], "list")==0) {
		command = COMMAND_LIST;
	}
	else if(strcmp(argv[1], "editors")==0) {
	}
	else if(strcmp(argv[1], "add")==0) {
		command = COMMAND_ADD;
	}
	else {
		throw std::runtime_error("invalid command");
	}

	read_config("config.ini");
}


void OptParser::read_config(const char* path)
{
	try {
		::libany::stdstream::IFileStream file(path);
		::libany::iniparser::Parser parser(file);

		char s[1024];
		char k[1024];
		char v[1024];
		while(parser.next_conf(s, sizeof(s), k, sizeof(k), v, sizeof(v))) {
			if(strcmp(s, "prodsrvcli")==0) {
				if(strcmp(k, "base_uri")==0) {
					strcpy(base_uri, v);
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

