#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define N 2

struct v
{
	int i;//Row
	int j;//Column
};

int A[N][N] = { { 1,2 },{ 3,4 } };//Matrix 1
int B[N][N] = { { 2,3 },{ 4,5 } };//Matrix 2
int C[N][N];//Resulting Matrix

static void * fnc(void *arg)
{
	struct v *data = (struct v *)arg;
	int l;
	for (l = 0; l<N; l++)
	{
		int i = ((struct v*)data[l]).i;//Row No
		int j = ((struct v*)data[l]).j;//Column No
		int accumulator = 0;
		int d, sum = 0;
		for (d = 0; d < N; d++)
		{
			sum = sum + A[i][d] * B[d][j];
		}
		C[i][j] = sum;
		sum = 0;
	}
	return;
}

int main()
{
	pthread_t threads[N];
	int i, k;
	for (i = 0; i<N; i++)
	{
		struct v *data[N];
		for (k = 0; k<N; k++)
		{
			data[k] = (struct v *)malloc(sizeof(struct v));
			data[k].i = i; //assign the row of C for thread to calculate
			data[k].j = k; //assign the column of C for thread to calculate
		}
		//In this example it creates 2 threads with passing data.
		//Data consists of row and column pairs for each thread, that will be calcuting the pairs.
		//Consider first iteration of this loop:
		//Thread 1 is created and data consists of (0,0) and (0,1) which are the targeted calculation cells for thread 1. 
		//In the second iteration: Thread 2 will have (1,0) and (1,1) pairs in its data.

		pthread_create(&threads[i], NULL, fnc, data);
	}
	for (i = 0; i < N; i++)
		pthread_join(threads[i], NULL);

	for (i = 0; i < N; i++)
		for (k = 0; k < N; k++)
			printf("%d\t", C[i][k]);

	return 0;
}