#include "reporter.h"
#include <time.h>
#include <vector>
#include "balance.h"

class Month {
	public:
	Balance balance;
	int mon;

	Month(AccTree& tree)
		: balance(tree) {};
};

class Year {
	public:
		int year;
		Balance balance;
		std::vector<Month> months;

		Year(AccTree& tree)
			: balance(tree) {};
};

static void 
adjust_year(std::vector<Year> &years, struct tm &ttm, AccTree& acctree)
{
	if(years.size() == 0 || years.back().year != ttm.tm_year) {
		years.push_back(Year(acctree));
		if(years.size() > 1) {
			years.back().balance = years[years.size()-2].balance;
			years.back().balance.close();
		}
		years.back().year = ttm.tm_year;
	}

}

static void 
adjust_month(std::vector<Year> &years, struct tm &ttm, AccTree& acctree)
{
	if(years.back().months.size() == 0 || years.back().months.back().mon != ttm.tm_mon) {
		years.back().months.push_back(Month(acctree));
		years.back().months.back().mon = ttm.tm_mon;
		if(years.back().months.size() == 1) {
			years.back().months.back().balance = years.back().balance;
		}
		else {
			years.back().months.back().balance = 
				years.back().months[years.back().months.size()-2].balance;
		}
		years.back().months.back().balance.close();
	}
}

void Reporter::make_report()
{
	unsigned int i;
	time_t now = time(NULL);
	std::vector<Year> years;
	struct tm ttm;

	for(i=0; i<_db.size(); i++) {
		localtime_r(&_db[i].ts, &ttm);
		adjust_year(years, ttm, _acctree);
		adjust_month(years, ttm, _acctree);

		if(_db[i].ts <= now && _db[i].forecast) {
			continue;
		}

		if(_db[i].ts > now && !_db[i].forecast) {
			continue;
		}
		
		years.back().months.back().balance.process(_db[i]);
		years.back().balance.process(_db[i]);
	}

	unsigned int j;
	unsigned int k;
	for(i=0; i<years.size(); i++) {
		/* header */
		fprintf(stdout, "account;");
		for(j=0; j<years[i].months.size(); j++) {
			fprintf(stdout, "%d;", years[i].months[j].mon+1);
		}
		fprintf(stdout, "%d\n", years[i].year+1900);
		
		/* body */
		bool skip;
		for(k=1; k<_acctree.accounts().size(); k++) {
			skip = false;
			if(!_opt.print_zero) {
				skip = true;
				for(j=0; j<years[i].months.size(); j++) {
					if(years[i].months[j].balance[k] != 0) {
						skip = false;
					}
				}
			}
			if(skip) continue;
			fprintf(stdout, "%s;", ((Account*)_acctree.accounts()[k])->id);
			for(j=0; j<years[i].months.size(); j++) {
				fprintf(stdout, "%3.2f;", years[i].months[j].balance[k]);
			}
			fprintf(stdout, "%3.2f\n", years[i].balance[k]);
		}

		fprintf(stdout, "\n");
	}
}

