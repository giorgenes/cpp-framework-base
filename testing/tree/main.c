#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int n = atoi(argv[1]);
	int p = atoi(argv[2]);

	int s;
	int c = 0;
	int a = 1;
	do {
		s = ((n-p + 2) * (n - p +1)) / 2;
		n--;
		c++;
		printf("%d %3.3f\n", s, (double)s / (double)a);
		a = s;
	}while(s > 1);
	printf("total: %d\n", c);
}
