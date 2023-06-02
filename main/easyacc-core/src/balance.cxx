#include "balance.h"
#include <stdexcept>

static struct {
	const char* type;
	double modifier;
}g_account_types[] = {
	{"cash",        1},
	{"asset",       1},
	{"bank",        1},
	{"income",     -1},
	{"expense",     1},
	{"equity",     -1},
	{"liability",  -1},
	{0, 0}
};


Balance::Balance(AccTree& tree)
	: _acctree(tree) 
{
	values.resize(_acctree.accounts().size(), 0);
}

double Balance::get_profit()
{
	double income = 0;
	double expense = 0;
	Node<Account>* n;
	
	for(unsigned int i=0; i<_acctree._acclist.size(); i++) {
		n = _acctree._acclist[i];
		if(n->down == 0) {
			if(((Account*)n)->type == ACC_TYPE_EXPENSE) {
				expense += values[i];
			}
			else if(((Account*)n)->type == ACC_TYPE_INCOME) {
				income += values[i];
			}

		}
	}
	
	return income - expense;
}


void Balance::close()
{
	if(!_acctree._close_account) {
		throw std::runtime_error("close account not specified");
	}
	
	double profit = get_profit();
	
	_acctree._close_account->credit(
			profit, 
			&values[_acctree._close_account->idx]);
	
	for(unsigned int i=0; i<_acctree._acclist.size(); i++) {
		if(((Account*)_acctree._acclist[i])->type == ACC_TYPE_INCOME 
		  || ((Account*)_acctree._acclist[i])->type == ACC_TYPE_EXPENSE) {
			values[i] = 0;
		}
	}
}

		
void Balance::debit(double val, unsigned int acc)
{
	Node<Account>* n;
	
	n = _acctree._acclist[acc];
	while(n != 0) {
		values[acc] += 
			val * g_account_types[((Account*)n)->type].modifier;

		n = n->up;
		if(n) {
			if(((Account*)n)->type != ((Account*)n->down)->type) {
				n = 0;
			}
			else {
				acc = ((Account*)n)->idx;
			}
		}
	}
}

void Balance::credit(double val, unsigned int acc)
{
	debit(-val, acc);
}


void Balance::process(const Transaction& t)
{
	debit(t.val, t.debit);
	credit(t.val, t.credit);
}

