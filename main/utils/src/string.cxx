#include <ctype.h>
#include <string.h>
#include "string_utils.h"

char* ::libany::utils::string_trimall(char* str, int* p_len)
{
	int   pos;
	char *p;

	/* comecamos removendo os espacos do final */
	if(p_len) {
		pos = *p_len - 1;
	}
	else {
		pos = strlen(str) - 1;
	}
	
	while((pos >= 0) && (isspace(str[pos]))) {
		str[pos--] = '\0';
	}

	/* agora procura o primeiro "nao-branco" do comeco */
	p = str;
	while((p != NULL) && (isspace(*p))) {
		++p;
		pos--;
	}

	if(p_len) {
		*p_len = pos + 1;
	}

	return p;
}


int 
::libany::utils::string_split(
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

