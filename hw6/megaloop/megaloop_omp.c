#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef double T;

T ** create_2d(int n, int m) {
  T * storage = malloc(n*m*sizeof(T));
  assert(storage);
  T ** rows = malloc(n*sizeof(T*));
  assert(rows);
  for (int i=0; i<n; i++)
    rows[i] = &storage[i*m];
  return rows;
}

void destroy_2d(T ** a) {
  free(a[0]); // free storage
  free(a); // free rows
}

int main(int argc, char * argv[]) {
  assert(argc == 4);
  int L = atoi(argv[1]), M = atoi(argv[2]), N = atoi(argv[3]);
  T ** a, ** b, ** c;
#pragma omp parallel
  {
#pragma omp sections
	  {
#pragma omp section
		  {
  			a = create_2d(N,L);
		  }
#pragma omp section
		  {
  			b = create_2d(L,M);
		  }
#pragma omp section
		  {
  			c = create_2d(N,M);
		  }
	  }
  }
#pragma omp parallel default(none) shared(a, N, L)
#pragma omp for collapse(2)
  for (int i=0; i<N; i++)
    for (int j=0; j<L; j++)
      a[i][j] = sin(i*L+j);
  for (int i=0; i<L; i++)
    for (int j=0; j<M; j++)
      b[i][j] = rand()*1.0/RAND_MAX;
#pragma omp parallel default(none) shared(a, b, c, N, M, L)
#pragma omp for collapse(2)
  for (int i=0; i<N; i++) {
    for (int j=0; j<M; j++) {
      T tmp = 0.0;
      for (int k=0; k<L; k++)
	tmp += a[i][k]*b[k][j]; //since tmp defined & declared within parallel region, no conflict
      c[i][j] = tmp;
    }
  }
  for (int i=0; i<N && i<M; i++) { // cannot use loop structure as this is in the wrong format
#pragma omp parallel for default(none) shared(c, i)
    for (int j=0; j<i; j++) {
      T tmp = c[i][j];
      c[i][j] = c[j][i];
      c[j][i] = tmp; //similarly, tmp defined within parallel construct so it is private
    }
  }
  T * p = c[0];
  T result = 0.0;
#pragma omp parallel for reduction(+:result) default(none) shared(N, M, p)
  for (int i=0; i<N*M; i++)
    result += p[i];
  printf("%lf\n", result);
  destroy_2d(a);
  destroy_2d(b);
  destroy_2d(c);
}
