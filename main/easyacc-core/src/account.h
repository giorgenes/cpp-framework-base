#ifndef __account_h
#define __account_h


enum AccountType {
	ACC_TYPE_CASH = 0,
	ACC_TYPE_ASSET,
	ACC_TYPE_BANK,
	ACC_TYPE_INCOME,
	ACC_TYPE_EXPENSE,
	ACC_TYPE_EQUITY,
	ACC_TYPE_LIABILITY
};

class Account {
	public:
		AccountType type;
		double value;
		char id[16];
		char name[64];
		unsigned int idx;

		void clean();

		void set_id(const char*);
		void set_name(const char*);
		void set_value(const char*);
		void set_type(const char*);
		void set_type(AccountType);
		const char* get_type();

		void debit(double, double* p =0);
		void credit(double, double* p =0);

		Account();
};


#endif
