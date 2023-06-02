#include <stdio.h>
#include <stdlib.h>

#define MAX 100

/* generate k-subsets of 1 ... n in alternating lexicographic order */
int main(int argc, char**argv)
{
	int i, j;
	int k, n, n_k;
	int *s;

	if(argc != 3) {
		fprintf(stderr, "usage: npr n r\n");
		exit(-1);
	}

	n = atoi(argv[1]);
	k = atoi(argv[2]);

	if(k > n) {
		fprintf(stderr, "n must be greater than r\n");
		return -1;
	}

	s = (int*)malloc((k+10) * sizeof(int));

	s[1] = 1;
	printf( "%d ", s[1] -1);
	n_k = n-k;

	for (i = 2; i <= k; i++)
	{
		s[i] = n_k+i;    /*initialize the first subset*/
		printf("%d ", s[i]-1);
	}
	printf("\n");

	j = 2;
	while (s[1] < n_k+1) {
		if (j > k) {
			s[k]++;
			if (s[k] == n) j-= 2;
		}
		else if (s[j-1] == s[j] - 1) {
			s[j-1] = s[j];
			s[j] = n_k + j;
			if (s[j-1] == s[j] - 1) j -= 2;
		}
		else {
			s[j]--;
			if (j < k)
			{
				s[j+1] = s[j] + 1;
				j += 2;
			}
		}
		/* we've got the next subset! */
		for (i = 1; i <= k; i++) {
			printf( "%d ", s[i]-1);
		}
		printf("\n");
	}
	return(0);
}

