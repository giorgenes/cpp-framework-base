#ifndef __acctree_h
#define __acctree_h

#include "tree.h"
#include "account.h"
#include <stdio.h>
#include <vector>

class AccTree : public Tree<Account> {
	private:
		Account* _close_account;
		std::vector<Node<Account>*> _acclist;
		void print_xml(FILE*);
		void print_csv(FILE*);
		std::vector<double>* _balance;

		double get_profit(std::vector<double>&);
	public:
		void load(const char*, std::vector<double>*);
		void print(const char*, const char*);
		void close();
		void close(std::vector<double>&);
		void process_transactions(const char*);
		void sumarize();

		Account* search(const char*);

		AccTree& operator -= (AccTree&);

		inline void set_close_account(Account* p) {
			_close_account = p;
		};

		inline Account* get_close_account(void) {
			return _close_account;
		};

		Node<Account>* add_child(Node<Account>*, Account&);

		inline std::vector<Node<Account>*>& accounts() {
			return _acclist;
		};

		void process(double, int, int, std::vector<double>&);

		AccTree();
		friend class Balance;
};

#endif
