/* Algorithm by Donald Knuth. */
/* C implementation by Glenn C. Rhoads 
 * Bug fix and hack by fixxxer ;-) 
 * */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int i, j=1, k, n, *c, x;

	if(argc != 3) {
		fprintf(stderr, "usage: npr n r\n");
		exit(-1);
	}

	n = atoi(argv[1]);
	k = atoi(argv[2]);

	if(k >= n) {
		fprintf(stderr, "n must be greater than r\n");
		return -1;
	}
	
	c = malloc( (k+3) * sizeof(int));

	for (i=1; i <=k; i++) c[i] = i;
	c[k+1] = n+1;
	c[k+2] = 0;
	j = k;

visit:
	for (i=k; i >= 1; i--) {
		printf("%d ", c[i]-1);
	}
	printf( "\n");

	if (j > 0) {
		x = j+1; 
		goto incr;
	}

	if (c[1] + 1 < c[2]) {
		c[1] += 1;
		goto visit;
	}

	j = 2;

do_more:
	c[j-1] = j-1;
	x = c[j] + 1;
	if (x == c[j+1]) {
		j++; 
		goto do_more;
	}

	if (j > k) exit(0);

incr:
	c[j] = x;
	j--;
	goto visit;

	return 0;
}

