#include "opt.h"
#include "acctree.h"
#include "customexcept.h"
#include "db.h"
#include "reporter.h"
#include <stdio.h>

static void process_transactions(Opt* p_opt, AccTree* p_tree)
{
	try {
		unsigned int i;

		for(i=0; i<p_opt->trans_files.size(); i++) {
			p_tree->process_transactions(p_opt->trans_files[i]);
		}
	}
	catch(...) {
		throw;
	}
}

int main(int argc, char** argv)
{
	try {
		Opt opt;

		opt.parse(argc, argv);
		opt.validate();

		AccTree tree;
		std::vector<double> balance;
		tree.load(opt.acc_trees[0], &balance);

		switch(opt.cmd) {
			case CMD_TRANSACTION:
				process_transactions(&opt, &tree);
				break;
			case CMD_CLOSE:
				tree.close();
				break;
			case CMD_DIFF:
				{
					//AccTree tree2;
					//tree2.load(opt.acc_trees[1]);
					//tree -= tree2;
				}
				break;
			case CMD_MAKEDB:
				{
					TransactionDb db(tree);
					db.make(opt);

					Reporter rep(db, tree, opt);
					rep.make_report();
				}
				break;
			case CMD_NONE:
				throw std::runtime_error("bug");
				break;
		}
		
		if(opt.cmd != CMD_MAKEDB) {
			tree.sumarize();
			tree.print(opt.out, opt.format);
		}
	}
	catch(CmdError& e) {
		Opt::usage();
		return -1;
	}
	catch(std::exception& e) {
		fprintf(stderr, "error: %s\n", e.what());
		return -2;
	}
	catch(...) {
		fprintf(stderr, "internal error\n");
		return -3;
	}
	return 0;
}

