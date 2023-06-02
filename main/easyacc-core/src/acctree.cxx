#include "acctree.h"
#include "utils.h"
#include "transparser.h"
#include <expat.h>
#include <stdexcept>
#include <vector>

static double get_sum_by_type(Node<Account>* p_root, AccountType p_type);
static double get_profit(AccTree& p_tree);

typedef struct {
	std::vector<Node<Account>* > stack;
	AccTree* tree;
}tree_load_aux_t;

AccTree::AccTree()
	: _close_account(0), _balance(0)
{
}

class AccTreeTransParser : public TransactionParser {
	private:
		AccTree& _acctree;
	protected:
		void on_transaction(const char*, const char*, double, bool, time_t);
	public:
		AccTreeTransParser(AccTree& tree)
			: _acctree(tree) {
		};
};

static void 
exec_transaction(
		AccTree* p_tree, 
		const char* p_debit, 
		const char* p_credit,
		double p_value)
{
	Account* acc[2];
	char aux[128];

	acc[0] = p_tree->search(p_debit);
	acc[1] = p_tree->search(p_credit);

	if(!acc[0]) {
		sprintf(aux, "account %s not found", p_debit);
		throw std::runtime_error(aux);
	}
	if(!acc[1]) {
		sprintf(aux, "account %s not found", p_credit);
		throw std::runtime_error(aux);
	}

	acc[0]->debit(p_value);
	acc[1]->credit(p_value);
}

void 
AccTreeTransParser::on_transaction(
		const char* p, const char* q, double value, bool future, time_t ts)
{
	/* we are not interested in future
	 * transactions here */
	if(future) return;

	try {
		exec_transaction(&_acctree, p, q, value);
	}
	catch(...) {
		throw;
	}
}


static void 
get_account_options(AccTree* p_tree, Account* p_acc, const char* p_opt)
{
	if(strcmp(p_opt, "close_account")==0) {
		p_tree->set_close_account(p_acc);
	}
}

static void 
get_account_properties(AccTree* p_tree, Account* p_acc, const char **attr)
{
	while(*attr) {
		if(strcmp(*attr, "id")==0) {
			p_acc->set_id(attr[1]);
		}
		else if(strcmp(*attr, "name")==0) {
			p_acc->set_name(attr[1]);
		}
		else if(strcmp(*attr, "value")==0) {
			p_acc->set_value(attr[1]);
		}
		else if(strcmp(*attr, "type")==0) {
			p_acc->set_type(attr[1]);
		}
		else if(strcmp(*attr, "opt")==0) {
			get_account_options(p_tree, p_acc, attr[1]);
		}

		attr += 2;
	}
}

static void start(void *data, const char *el, const char **attr)
{
	Node<Account>* n;
	Account acc;
	tree_load_aux_t* aux = (tree_load_aux_t*)data;

	if(strcmp(el, "account")==0) {
		acc.clean();

		/* atribui o tipo do pai
		 * ao filho */
		acc.set_type(((Account&)(*aux->stack.back())).type);

		n = aux->tree->add_child(aux->stack.back(), acc);
		get_account_properties(aux->tree, (Account*)n, attr);
		aux->stack.push_back(n);
	}
}

static void end(void *data, const char *el)
{
	if(strcmp(el, "account")==0) {
		tree_load_aux_t* aux = (tree_load_aux_t*)data;
		aux->stack.pop_back();
	}
}


static void XML_chardata(void *data,const XML_Char *s,int len)
{
}

void AccTree::load(const char* p_path, std::vector<double>* balance)
{
	try {
		XML_Parser parser;
		File fp;
		tree_load_aux_t aux;

		aux.tree = this;
		_balance = balance;

		fp.open(p_path, "r");

		parser = XML_ParserCreate("ISO-8859-1");
		if(!parser) {
			throw std::runtime_error("could not create parser");
		}

		XML_SetElementHandler(parser, start, end);
		XML_SetCharacterDataHandler(parser, XML_chardata);
		XML_SetUserData(parser, &aux);

		char buf[1024];
		int len;

		/* insere a raiz */
		Account root;
		aux.stack.push_back(add_child(0, root));

		while(!feof(fp)) {
			if((len=fread(buf, 1, sizeof(buf), fp)) <= 0) break;
			XML_Parse(parser, buf, len, 0);
		}

		XML_ParserFree(parser);
	}
	catch(...) {
		throw;
	}
}

void AccTree::print(const char* p_path, const char* p_format)
{
	try {
		File fp;

		fp.open(p_path, "w");

		if(strcmp(p_format, "xml")==0) {
			print_xml(fp);
		}
		else if(strcmp(p_format, "csv")==0) {
			print_csv(fp);
		}
		else {
			throw std::runtime_error("unsupported output format");
		}
	}catch(...) {
		throw;
	}
}

static void print_csv_node(Node<Account>* p_root, FILE* p_fp)
{
	if(!p_root) return;

	print_csv_node(p_root->next, p_fp);

	fprintf(p_fp,
			"%s;%s;%3.2f;%s\n",
			((Account*)p_root)->id,
			((Account*)p_root)->name,
			((Account*)p_root)->value,
			((Account*)p_root)->get_type());

	print_csv_node(p_root->down, p_fp);
}

void AccTree::print_csv(FILE* p_fp)
{
	print_csv_node(_root->down, p_fp);
	fprintf(p_fp, "profit;profit;%3.3f;profit\n", ::get_profit(*this));
}

static void 
print_xml_node(AccTree* self, Node<Account>* p_root, int p_level, FILE* p_fp)
{
	if(!p_root) return;

	print_xml_node(self, p_root->next, p_level, p_fp);

	for(int i = 0; i<p_level; i++) {
		fprintf(p_fp, "\t");
	}
	fprintf(p_fp, "<account "
			"id=\"%s\" "
			"name=\"%s\" "
			"value=\"%3.2f\" "
			"type=\"%s\"",
			((Account*)p_root)->id,
			((Account*)p_root)->name,
			((Account*)p_root)->value,
			((Account*)p_root)->get_type());

#if 0	
	fprintf(stderr, "close_acc=%p\n", self->get_close_account());
	fprintf(stderr, "root=%p\n", p_root);
	fprintf(stderr, "acc=%p\n", &(p_root->get_data()));
#endif

	if(self->get_close_account() == &(p_root->get_data())) {
		fprintf(p_fp, " opt=\"close_account\"");
	}
	
	if(!p_root->down) {
		fprintf(p_fp, "/");
	}
	fprintf(p_fp, ">\n");

	print_xml_node(self, p_root->down, p_level+1, p_fp);

	if(p_root->down) {
		for(int i = 0; i<p_level; i++) {
			fprintf(p_fp, "\t");
		}
		fprintf(p_fp, "</account>\n");
	}
}

void AccTree::print_xml(FILE* p_fp)
{
	fprintf(p_fp, "<accounts>\n");
	print_xml_node(this, _root->down, 1, p_fp);
	fprintf(p_fp, "</accounts>\n");
}


static void substract_node(Node<Account>* p_root, AccTree* p_other)
{
	if(!p_root) return;

	Account* acc = p_other->search(((Account*)p_root)->id);
	if(acc) {
		((Account*)p_root)->value -= acc->value;
	}

	substract_node(p_root->down, p_other);
	substract_node(p_root->next, p_other);
}

AccTree& AccTree::operator -= (AccTree& p_other)
{
	substract_node(_root->down, &p_other);
	return *this;
}

static double get_sum_by_type(Node<Account>* p_root, AccountType p_type)
{
	if(!p_root) return 0;
	
	double sum = 0;
	
	if(((Account*)p_root)->type == p_type) {
		sum += ((Account*)p_root)->value;
	}

	sum += get_sum_by_type(p_root->down, p_type);
	sum += get_sum_by_type(p_root->next, p_type);

	return sum;
}

static double get_profit(AccTree& p_tree)
{
	double income;
	double expense;

	income  = get_sum_by_type(p_tree._root->down, ACC_TYPE_INCOME);
	expense = get_sum_by_type(p_tree._root->down, ACC_TYPE_EXPENSE);

	return income - expense;
}

static void 
set_value_by_type(Node<Account>* p_root, AccountType p_type, double p_value)
{
	if(!p_root) return;
	
	if(((Account*)p_root)->type == p_type) {
		((Account*)p_root)->value = p_value;
	}

	set_value_by_type(p_root->down, p_type, p_value);
	set_value_by_type(p_root->next, p_type, p_value);
}


void AccTree::close()
{
	if(!_close_account) {
		throw std::runtime_error("close account not specified");
	}
	
	double profit = ::get_profit(*this);
	
	_close_account->credit(profit);
	
	set_value_by_type(_root->down, ACC_TYPE_INCOME,  0);
	set_value_by_type(_root->down, ACC_TYPE_EXPENSE, 0);
}

static Account* search_node(Node<Account>* p_root, const char* p_id)
{
	if(!p_root) return 0;

	if(strcmp(((Account*)p_root)->id, p_id)==0) {
		return (Account*)p_root;
	}

	Account* acc;

	acc = search_node(p_root->down, p_id);
	if(acc) return acc;

	acc = search_node(p_root->next, p_id);

	return acc;
}

/* TODO: optimize this function to use
 * a hash or something faster than
 * a linear search */
Account* AccTree::search(const char* p_id)
{
	const char* p;
	if((p=strchr(p_id, '.'))) {
		Node<Account>* cur;
		bool found;

		cur = _root;
		do {
			p = strchr(p_id, '.');
			if(!p) {
				p = &p_id[strlen(p_id)];
			}
			found = false;
			cur = cur->down;
			while(cur) {
				if(strncmp(((Account*)cur)->id, p_id, p - p_id)==0) {
					found = true;
					break;
				}
				cur = cur->next;
			}
			if(!found) {
				return 0;
			}
			p_id = p+1;
		}while(*p != 0);
		return (Account*)cur;
	}
	else {
		return search_node(_root->down, p_id);
	}
}


		
void AccTree::process_transactions(const char* p_path)
{
	try {
		AccTreeTransParser parser(*this);

		parser.process_file(p_path);
	}
	catch(...) {
		throw;
	}
}

void AccTree::sumarize()
{
}

Node<Account>* AccTree::add_child(Node<Account>* root, Account& acc)
{
	Node<Account>* n = Tree<Account>::add_child(root, acc);

	((Account*)n)->idx = _acclist.size();
	_acclist.push_back(n);
	return n;
}

void 
AccTree::process(double val, int debit, int credit, std::vector<double>& array)
{
	((Account*)_acclist[debit])->debit(val, &array[debit]);
	((Account*)_acclist[credit])->credit(val, &array[credit]);
}


double AccTree::get_profit(std::vector<double>& balance)
{
}

void 
AccTree::close(std::vector<double>& balance)
{
	if(!_close_account) {
		throw std::runtime_error("close account not specified");
	}
	
	double profit = get_profit(balance);
	
	_close_account->credit(profit);
	
	set_value_by_type(_root->down, ACC_TYPE_INCOME,  0);
	set_value_by_type(_root->down, ACC_TYPE_EXPENSE, 0);

}

