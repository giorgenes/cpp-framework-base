#include <stdio.h>
#include <vector>
#include <sstream>
#include <cstring>
#include <libany/utils/string_utils.h>
#include "strfile.h"

class Buffer {
	private:
		unsigned int __size;
		unsigned int __max;
		char* __buf;

	public:

		Buffer()
			: __size(0), __max(0), __buf(0)
		{
		};

		void grow(unsigned);
		void append(const void*, unsigned int);

		inline unsigned int size() const {
			return __size;
		};

		inline const char* buf() const {
			return __buf;
		};
};

void Buffer::grow(unsigned int len)
{
	if(__size + len > __max) {
		__max += len;
		__max *= 2;
		__buf = static_cast<char*>(realloc(__buf, __max));
	}
}
		
void Buffer::append(const void* buf, unsigned int size)
{
	grow(size);
	std::memcpy(__buf + __size, buf, size);
	__size += size;
}

class Table {
	private:
		Buffer __keys;
		Buffer __buf;
		std::vector<unsigned int> __keyidx;
		std::vector<unsigned int> __bufidx;
	public:
		void add(const char*, std::string&);
		void dump();
};

void Table::add(const char* key, std::string& buf)
{
	if(!*key) return;
	
	fprintf(stderr, "%s\n", key);
	unsigned int size;
	
	__keyidx.push_back(__keys.size());
	__bufidx.push_back(__buf.size());
	
	size = std::strlen(key);
	__keys.append(&size, sizeof(size));
	__keys.append(key, size+1);

	size = buf.size();
	char* p = new char[size+1];
	std::strcpy(p, buf.c_str());
	char* q = ::libany::utils::string_trimall(p, (int*)&size);
	__buf.append(&size, sizeof(size));
	__buf.append(q, size+1);
	fprintf(stderr, "%s\n", q);
	delete p;
}

void Table::dump()
{
	unsigned int i;
	::libany::strfile::header_t header;

	header.n = __keyidx.size();
	header.idxb = sizeof(header);
	header.keyb = header.idxb + (sizeof(unsigned) * 2 * __keyidx.size());
	header.datab = header.keyb + __keys.size();
	
	
	/* write header */
	fwrite(&header, sizeof(header), 1, stdout);

	unsigned int s;
	/* the key index arrays */	
	for(i=0; i<__keyidx.size(); i++) {
		s = __keyidx[i];
		fwrite(&s, 1, sizeof(s), stdout);
	}

	/* the buf index arrays */	
	for(i=0; i<__bufidx.size(); i++) {
		s = __bufidx[i];
		fwrite(&s, 1, sizeof(s), stdout);
	}

	/* the keys */
	fwrite(__keys.buf(), 1, __keys.size(), stdout);
	
	/* the data */
	fwrite(__buf.buf(), 1, __buf.size(), stdout);
}


int main()
{
	char buf[1024];
	char* b;
	char* e;
	char key[1024] = "";
	std::string s;
	int len;
	Table table;
	
	while(fgets(buf, sizeof(buf), stdin)!=NULL) {
		b = strstr(buf, "[[[");
		e = strstr(buf, "]]]");
		if(b && e && e > b) {
			*e = 0;
			b += 3;

			len = e - b;
			table.add(key, s);
			strcpy(key, b);
			s = "";
		}
		else {
			s += buf;
		}
	}
	table.add(key, s);
	table.dump();
}

