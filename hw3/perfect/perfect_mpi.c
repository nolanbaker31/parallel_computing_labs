/* perfect.c: find all perfect numbers up to a bound */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

double mytime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + t.tv_usec/1000000.0;
}

_Bool is_perfect(int n) {
  if (n < 2) return 0;
  int sum = 1, i = 2;
  while (1) {
    const int i_squared = i*i;
    if (i_squared < n) {
      if (n%i == 0) sum += i + n/i;
      i++;
    } else {
      if (i_squared == n) sum += i;
      break;
    }
  }
  return sum == n;
}

int main(int argc, char * argv[]) {
  MPI_Init(&argc, &argv);
  int rank;
  int nprocs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  int num_perfect = 0;
  double start_time = mytime();
  if (argc != 2) {
    //printf("Usage: perfect.exec bound\n");
    exit(1);
  }
  int bound = atoi(argv[1]);
  for (int i=rank+1; i<=bound; i+=nprocs) {
    if (i%1000000 == 0) {
      //printf("i = %d\n", i);
      //fflush(stdout);
    }
    if (is_perfect(i)) {
      //printf("Found a perfect number: %d\n", i);
      //fflush(stdout);
      num_perfect++;
    }
  }
  int num_perfect_total;
  MPI_Reduce(&num_perfect, &num_perfect_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if(rank == 0){
  /*printf("Number of perfect numbers less than or equal to %d: %d.  Time = %lf\n",
	 bound, num_perfect_total, mytime() - start_time);*/
	  printf("%d %lf\n", nprocs, mytime() - start_time);
	  fflush(stdout);
  }
  MPI_Finalize();
}
