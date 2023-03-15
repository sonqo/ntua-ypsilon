#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
	
    int i;
	int N = 100000000;
	double *x1, *x2, *x3, *y;
	double a1 = 0.5;
	double a2 = 1.0;
	double a3 = 1.5;

    struct timeval start, finish;

	y = (double*) malloc(N*sizeof(double));
	x1 = (double*) malloc(N*sizeof(double));
	x2 = (double*) malloc(N*sizeof(double));
	x3 = (double*) malloc(N*sizeof(double));

	for (i=0; i<=N-1; i++) {
		y[i] = 0;
		x1[i] = (double) i * 0.5;
		x1[i] = (double) i * 0.4; 
		x2[i] = (double) i * 0.8;
		x3[i] = (double) i * 0.2;
	}

    gettimeofday(&start, NULL);
	for (i=0; i<=N-1; i++) {
		y[i] = y[i] + a1*x1[i] + a2*x2[i] + a3*x3[i];
	}
    gettimeofday(&finish, NULL);

    printf("%d", finish.tv_usec-start.tv_usec);

	return 0;

}