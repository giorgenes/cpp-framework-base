#ifndef __opt_h
#define __opt_h

#define COMMAND_NONE     -1
#define COMMAND_CREATE    0
#define COMMAND_CHECKOUT  1
#define COMMAND_LIST      2
#define COMMAND_ADD       3
#define COMMAND_MV        5
#define COMMAND_RM        6
#define COMMAND_COMMIT    7
#define COMMAND_LISTREV   8

class OptParser {
	private:
		void read_config(const char*);
	public:
		void show_help();
		void parse(int, char**);

		int command;
		char name[128];
		char base_uri[2048];
		char brand[128];
		char service[128];
		char version[64];

		OptParser();
};

#endif
