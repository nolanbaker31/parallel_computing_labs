#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <time.h>

int main(){
	MPI_Init(NULL, NULL);
	int nprocs, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int arr[5];
	printf("Process %d has:                ", rank);
	fflush(stdout);
	srand(time(NULL) + rank);
	for(int i=0; i<5; i++){
		arr[i] = rand()%1000;
		printf("   %d   ", arr[i]);
		fflush(stdout);
	}
	int prevThreadRand[5];
	const int right = (rank+1)%nprocs;
	const int left = (rank+nprocs-1)%nprocs;
	MPI_Sendrecv(arr, 5, MPI_INT, right, 0, prevThreadRand, 5, MPI_INT, left, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	printf("\nProcess %d received:           ", rank);
	fflush(stdout);
	for(int i = 0; i < 5; i++){
		printf("   %d   ", prevThreadRand[i]);
		fflush(stdout);
	}
	printf("\n");
	MPI_Finalize();
	return 0;
}
