#ifndef __balance_h
#define __balance_h

#include "acctree.h"
#include <vector>

#include "db.h"

class Balance {
	private:
		AccTree& _acctree;
		std::vector<double> values;
	public:
		void close();

		Balance(AccTree&);

		inline operator std::vector<double>& () {
			return values;
		};

		inline Balance& operator = (const Balance& b) {
			values = b.values;
			return *this;
		};

		inline double operator [] (unsigned int idx) {
			return values[idx];
		};

		void process(const Transaction&);
		double get_profit();
		void debit(double val, unsigned int account);
		void credit(double val, unsigned int account);
};

#endif
