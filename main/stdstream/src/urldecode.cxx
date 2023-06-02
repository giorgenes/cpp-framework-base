#include "urldecode.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace ::libany::stdstream;

int UrlDecodeStream::read(char* vbuf, int s)
{
	if(_end) return 0;
	
	char* p = (char*)vbuf;
	char* buf = (char*)vbuf;
	int len = _st.read(buf, s);
	int b = 0;
	while(p < &((char*)buf)[len]) {
		if(*p++ == '&') {
			_st.unread(p, len - (p - (char*)buf));
			len = (p - (char*)buf) -1;
			_end = true;
			break;
		}
	}

#if 0
	fprintf(stdout, "buf: ");
	fwrite(buf, len, sizeof(char), stdout);
	fprintf(stdout, "\n");
#endif

	p = (char*)buf;
	for(int i=0; i<len; i++) {
		if(buf[i] == '+') {
			*p++ = ' ';
			b++;
		}
		else if(buf[i] == '%') {
			if(i+3 > len) {
				_st.unread(&buf[i], len - i);
				break;
			}
			else {
				char aux[3];
				aux[0] = buf[++i];
				aux[1] = buf[++i];
				aux[2] = 0;
				*p++ = strtol(aux, NULL, 16);
				//i += 2;
				//*p++ = '?';
				b++;
			}
		}
		else {
			*p++ = buf[i];
			b++;
		}
	}

	/* this should not be necessary.
	 * it's here to avoid problems
	 * when the dumb user like me forgets
	 * to finish the string with a \0 */
	if(b < s) {
		p = static_cast<char*>(vbuf);
		p[b] = 0;
	}
	return b;
	
}

bool UrlDecodeStream::eos()
{
	if(_end) {
		return true;
	}
	
	return _st.eos();
}


bool UrlDecodeStream::get_next_pair()
{
	int len = _st.read(_key, sizeof(_key)-1);
	if(len <= 0) return false;
	_key[len] = 0;
	
	char* p = strchr(_key, '=');
	if(!p) return false;

	*p++ = 0;
	_st.unread(p, len - (p - _key));

	_end = false;
	return true;
}

