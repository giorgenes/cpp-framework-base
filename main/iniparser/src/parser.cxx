#include <libany/iniparser/parser.h>
#include <libany/utils/string_utils.h>
#include <string.h>
#include <stdexcept>

using namespace ::libany::iniparser;

Parser::~Parser()
{
}

void Parser::on_conf(const char*, const char*, const char*)
{
}

void Parser::parse()
{
	char s[1024];
	char k[1024];
	char v[1024];
	
	while(next_conf(s, sizeof(s), k, sizeof(k), v, sizeof(v))) {
		on_conf(s, k, v);
	}
}

inline void throw_error(int line)
{
	char buf[1024];

	sprintf(buf, "parser error on line: %d", line);
	throw std::runtime_error(buf);
}

bool Parser::next_conf(
		char* session, int session_size, 
		char* key, int key_size, 
		char* val, int val_size)
{
	char buf[LIBANY_STDSTREAM_BUFSIZE];
	int len;
	bool found_key = false;
	char* p;

	while(!found_key) {
		if((len=_st.readline(buf, sizeof(buf)))<=0) {
			return false;
		}
		//fprintf(stderr, "%s", buf);
		_line++;
		p = ::libany::utils::string_trimall(buf, &len);
		if(*p == 0 || *p == ';') continue;
		if(len < 3) {
			throw_error(_line);
		}

		if(*p == '[') {
			if(p[len-1] != ']') {
				throw_error(_line);
			}
			len--;
			p[len] = 0;
			strncpy(_last_session, p+1, sizeof(_last_session));
			_last_session[sizeof(_last_session)-1] = 0;
		}
		else {
			char* m;
			m = strchr(p, '=');
			if(!m) {
				throw_error(_line);
			}

			*m++ = 0;
			::libany::utils::string_trimall(p, 0);
			m = ::libany::utils::string_trimall(m, 0);

			strncpy(key, p, key_size);
			key[key_size-1] = 0;
			strncpy(val, m, val_size);
			val[val_size-1] = 0;
			found_key = true;
		}
	}

	if(found_key) {
		strncpy(session, _last_session, session_size);
		return true;
	}
	
	return false;
}


