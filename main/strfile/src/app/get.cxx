#include "strfile.h"

int main(int argc, char** argv)
{
	::libany::strfile::StrFile sf;

	sf.load(argv[1]);

	sf.loadstr(argv[2]);

	printf("%s\n", sf.str());
	
	return 0;
}
