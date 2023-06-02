#ifndef __transdb_h
#define __transdb_h

#include "opt.h"
#include "acctree.h"
#include "transparser.h"
#include "utils.h"
#include <vector>

class Transaction {
	public:
	time_t ts;
	double val;
	unsigned int debit;
	unsigned int credit;
	bool forecast;
};

class TransactionDb : private TransactionParser {
	private:
		AccTree& _tree;
		std::vector<Transaction> _transactions;
		
	protected:
		void on_transaction(const char*, const char*, double, bool, time_t);
	public:
		void make(Opt& p_opt);

		TransactionDb(AccTree& tree)
			: _tree(tree) {
		};

		inline Transaction& operator[](unsigned int idx) {
			return _transactions[idx];
		};

		inline unsigned int size() {
			return _transactions.size();
		};
};

#endif
