#include "opt.h"
#include "customexcept.h"
#include <getopt.h>
#include <stdio.h>

Opt::Opt()
	:
	cmd(CMD_NONE),
	out("-"),
	format("xml"),
	print_zero(false)
{
	acc_trees[0] = "-";
	acc_trees[1] = NULL;
}

void Opt::parse(int argc, char** argv)
{
	int c;
	struct option long_options[] = {
		{"process", 0, 0, 'p'},
		{"close",   0, 0, 'c'},
		{"somet",   1, 0, 's'},
		{"output",  1, 0, 'o'},
		{"acctree", 1, 0, 'a'},
		{"format",  1, 0, 'f'},
		{"makedb",  0, 0, 'm'},
		{"zero",  0, 0, 'z'},
		{0, 0, 0, 0}
	};

	while (1) {
		int option_index = 0;

		c = getopt_long (argc, argv, "pcs:o:a:f:mz",
				long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'p':
				if(this->cmd != CMD_NONE) {
					throw 
					CmdError("incompatible param types");
				}
				this->cmd = CMD_TRANSACTION;
				break;
			case 'c':
				if(this->cmd != CMD_NONE) {
					throw 
					CmdError("incompatible param types");
				}
				this->cmd = CMD_CLOSE;
				break;
			case 's':
				if(this->cmd != CMD_NONE) {
					throw 
					CmdError("incompatible param types");
				}
				this->acc_trees[1] = optarg;
				this->cmd = CMD_DIFF;
				break;
			case 'm':
				if(this->cmd != CMD_NONE) {
					throw 
					CmdError("incompatible param types");
				}
				this->cmd = CMD_MAKEDB;
				break;
			case 'o':
				this->out = optarg;
				break;
			case 'z':
				this->print_zero = true;
				break;
			case 'a':
				this->acc_trees[0] = optarg;
				break;
			case 'f':
				this->format = optarg;
				break;
			default:
				throw CmdError("unknow option");
				break;
		}
	}

	while (optind < argc) {
		this->trans_files.push_back(argv[optind++]);
	}

	if(this->cmd == CMD_NONE) {
		this->cmd = CMD_TRANSACTION;
	}
}

void Opt::usage()
{
	fprintf(stderr, "usage:\n");
	fprintf(stderr, "\teacc [-paof] [transaction files]\n");
	fprintf(stderr, "\teacc -c [-aof]\n");
	fprintf(stderr, "\teacc -s account_tree_file [-aof] \n");
	fprintf(stderr, "options:\n");
	fprintf(stderr, "\t-c       close the balance\n");
	fprintf(stderr, "\t-p       process transactions (default mode)\n");
	fprintf(stderr, "\t-m       make database to stdout\n");
	fprintf(stderr, "\t-s file  \n");
	fprintf(stderr, "\t-a file  use file as the account tree\n");
	fprintf(stderr, 
		"\t-o file  output the resulting account tree to file\n");

	fprintf(stderr, 
		"\t-f file  output the resulting account tree using format\n");

}

void Opt::validate()
{
}

