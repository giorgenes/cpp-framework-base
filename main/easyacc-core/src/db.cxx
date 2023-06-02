#include "db.h"
#include <stdexcept>
#include <algorithm>

bool operator< (const Transaction& a, const Transaction& b)
{
	return a.ts < b.ts;
}

void 
TransactionDb::on_transaction(
		const char* debit,
		const char* credit,
		double val,
		bool forecast,
		time_t ts)
{
	Transaction rec;

	Account* acc[2];
	char aux[128];

	acc[0] = _tree.search(debit);
	acc[1] = _tree.search(credit);

	if(!acc[0]) {
		sprintf(aux, "account %s not found", debit);
		throw std::runtime_error(aux);
	}
	if(!acc[1]) {
		sprintf(aux, "account %s not found", credit);
		throw std::runtime_error(aux);
	}

	rec.debit = acc[0]->idx;
	rec.credit = acc[1]->idx;

	rec.val = val;
	rec.ts = ts;
	rec.forecast = forecast;

	_transactions.push_back(rec);
}

void TransactionDb::make(Opt& p_opt)
{
	try {
		unsigned int i;
		
		for(i=0; i<p_opt.trans_files.size(); i++) {
			process_file(p_opt.trans_files[i]);
		}

		std::sort(_transactions.begin(), _transactions.end());

#if 0
		for(i=0; i<_transactions.size(); i++) {
			fprintf(stderr, "%d %d %3.3f %d\n", 
					_transactions[i].debit,
					_transactions[i].credit,
					_transactions[i].val,
					_transactions[i].ts);
		}
#endif
	}
	catch(...) {
		throw;
	}
}

