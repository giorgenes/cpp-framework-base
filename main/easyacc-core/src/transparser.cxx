#include "transparser.h"
#include <stdio.h>
#include<time.h>
#include <stdlib.h>

#include <stdexcept>
#include "utils.h"

/* TODO: usar o componente utils aqui e
 * remover essas funcoes */
static char *trimall(char *str)
{
    int   pos;
    char *p;

    if((str == NULL) || (strlen(str) == 0))
        return str;

    /* comecamos removendo os espacos do final */
    pos = strlen(str) - 1;

    while((pos >= 0) && (isspace(str[pos]))) {
        str[pos--] = '\0';
    }

    /* agora procura o primeiro "nao-branco" do comeco */
    p = str;
    while((p != NULL) && (isspace(*p))) {
        ++p;
    }
    return p;
}

static int 
string_split(
		char* s, 
		const char* p_sep, 
		char** args, 
		int max)
{
	char* p;
	int n = 0;
	bool hnull = !isspace(*p_sep);

	char* aux = s;
	while(max > 0 && (p = strsep(&aux, p_sep))) {
		if(*p || hnull) {
			*args++ = p;
			n++;
			max--;
		}
	}

	return n;
}

void TransactionParser::process_auto_command(char** args)
{
	int n = atoi(args[3]);
	
	time_t now = time(0);
	struct tm tmd;
	
	localtime_r(&now, &tmd);
	tmd.tm_mday = 28;	
	while(n--) {		
		if(tmd.tm_mon == 11){
			tmd.tm_mon = 0;
		}
		else {
			tmd.tm_mon ++ ;
		}

		now = mktime(&tmd);

		on_transaction(args[1], args[2], atof(args[0]), true, now);
	}
}

void TransactionParser::parse_command(char* cmd, struct tm* ttm)
{
	char c;
	int val;
	char* args[10];
	int n;

	n = string_split(cmd, " \t", args, 10);
	if(n == 2 ) {
		c = *args[0];
		val = atoi(args[1]);
		switch(c) {
			case 'y':
				ttm->tm_year = val - 1900;
				ttm->tm_mon = 0;
				ttm->tm_mday = 1;
				break;
			case 'm':
				ttm->tm_mon = val - 1;
				ttm->tm_mday = 1;
				break;
			case 'd':
				ttm->tm_mday = val;
				break;
		}
	}
	else if(n == 5) {
		if(strcmp(args[0], "auto")==0) {
			process_auto_command(&args[1]);
		}
	}
}

/*
 * decided to use an ! in front of the transaction
 * to specify future transactions instead of a single mark
 * splitting the file in 2 because the user may
 * want to use more than one file and the files
 * may not be sorted.
 *
 * we could use the date instead, to know what is future
 * but doing this way assure that we visually recognize what
 * is future and avoid a future transaction to be automatically
 * processed when it should not be.
 * */
void TransactionParser::process_file(const char* p_path)
{
	try {
		File fp;

		fp.open(p_path, "r");

		char buf[1024];
		char* p;
		char* q;
		bool error = false;
		int line=0;
		double value;
		bool forecast;
		time_t ts = 0;
		struct tm ttm;

		memset(&ttm, 0, sizeof(ttm));
		ttm.tm_mday = 1;
		while(!feof(fp)) {
			line++;

			if(fgets(buf, sizeof(buf), fp)==NULL) break;
			p = strchr(buf, '#');
			if(p) *p = 0;

			p = trimall(buf);
			if(!*p) continue;

			
			if(isalpha(*p)) {
				parse_command(p, &ttm);
				ts = mktime(&ttm);
			}
			else {
				forecast = false;
				if(*p == '!') {
					forecast = true;
					p++;
				}

				q = strchr(p, ' ');
				if(!q) { error = true; break; }
				*q = 0;
				value = atof(p);
				p = trimall(q+1);
				if(!*p) { error = true; break; }
				q = strchr(p, ' ');
				if(!q) { error = true; break; }
				*q = 0;
				q = trimall(q+1);
				if(!*q) { error = true; break; }

				on_transaction(p, q, value, forecast, ts);
			}
		}
		if(error) {
			sprintf(buf, 
					"parser error on file '%s', line %d", p_path, line);
			throw std::runtime_error(buf);
		}
	}
	catch(...) {
		throw;
	}
}

		
void 
TransactionParser::on_transaction(const char*, const char*, double, bool, time_t)
{
}

