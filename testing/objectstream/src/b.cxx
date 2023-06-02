#include <libany/objectstream/stream.h>
#include <libany/objectstream/types.h>
#include <libany/stdstream/filestream.h>
#include <stdio.h>

namespace vs {
	using namespace ::libany::stdstream;
	using namespace ::libany::estp;
}
	using namespace ::libany::estp;

int main()
{
	vs::IOFileStream obj1(stdin);

	int val;
	obj1 >> val;

	printf("val = %d\n", val);
}


