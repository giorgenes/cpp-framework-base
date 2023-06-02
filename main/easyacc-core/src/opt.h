#ifndef __opt_h
#define __opt_h

#include <vector>

enum OptCommand {
	CMD_TRANSACTION,
	CMD_CLOSE,
	CMD_DIFF,
	CMD_MAKEDB,
	CMD_NONE
};

class Opt {
	public:
		OptCommand cmd;
		const char* acc_trees[2];
		const char* out;
		const char* format;
		const char* dbfile;
		bool print_zero;
		std::vector<const char*> trans_files;

		void parse(int, char**);
		static void usage();
		void validate();

		Opt();
};

#endif
