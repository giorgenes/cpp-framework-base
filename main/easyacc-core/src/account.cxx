#include "account.h"
#include <string.h>
#include <stdlib.h>

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

Account::Account()
{
	clean();
}

void Account::clean()
{
	*this->name = 0;
	*this->id   = 0;
	this->value = 0;
	this->type  = ACC_TYPE_CASH;
}

void Account::set_id(const char* p)
{
	strcpy(this->id, p);
}

void Account::set_name(const char* p)
{
	strcpy(this->name, p);
}

void Account::set_value(const char* p)
{
	this->value = atof(p);
}

const char* Account::get_type()
{
	return g_account_types[this->type].type;
}

void Account::set_type(const char* p)
{
	int i = 0;
	
	while(g_account_types[i].type) {
		if(strcmp(p, g_account_types[i].type)==0) {
			this->set_type((AccountType)i);
			return;
		}
		i++;
	}
}

void Account::set_type(AccountType p)
{
	this->type = p;
}

void Account::debit(double v, double* p)
{
	if(p) {
		*p += (v * g_account_types[this->type].modifier);
	}
	else {
		this->value += (v * g_account_types[this->type].modifier);
	}
}

void Account::credit(double v, double* p)
{
	if(p) {
		*p -= (v * g_account_types[this->type].modifier);
	}
	else {
		this->value -= (v * g_account_types[this->type].modifier);
	}
}


