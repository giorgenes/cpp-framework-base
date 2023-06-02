#ifndef __reporter_h
#define __reporter_h

#include "db.h"
#include "acctree.h"
#include "opt.h"

class Reporter {
	protected:
		TransactionDb& _db;
		AccTree& _acctree;
		Opt& _opt;
	public:
		Reporter(TransactionDb& db, AccTree& tree, Opt& opt)
			: _db(db), _acctree(tree), _opt(opt) {
		};
		void make_report();
};

#endif
