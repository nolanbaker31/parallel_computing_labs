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

  a = create_2d(N,L);
  b = create_2d(L,M);
  c = create_2d(N,M);
  for (int i=0; i<N; i++)
    for (int j=0; j<L; j++)
      a[i][j] = sin(i*L+j);
  for (int i=0; i<L; i++)
    for (int j=0; j<M; j++)
      b[i][j] = rand()*1.0/RAND_MAX;
  for (int i=0; i<N; i++) {
    for (int j=0; j<M; j++) {
      T tmp = 0.0;
      for (int k=0; k<L; k++)
	tmp += a[i][k]*b[k][j];
      c[i][j] = tmp;
    }
  }
  for (int i=0; i<N && i<M; i++) {
    for (int j=0; j<i; j++) {
      T tmp = c[i][j];
      c[i][j] = c[j][i];
      c[j][i] = tmp;
    }
  }
  T * p = c[0];
  T result = 0.0;
  for (int i=0; i<N*M; i++)
    result += p[i];
  printf("%lf\n", result);
  destroy_2d(a);
  destroy_2d(b);
  destroy_2d(c);
}
