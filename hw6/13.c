#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define n 5
int *p, *q;
int main() {
#pragma omp parallel sections
{
#pragma omp section
{
p = malloc(n*sizeof(int));
assert(p);
for (int i=0; i<n; i++) p[i] = i;
}
#pragma omp section
{
q = malloc(n*sizeof(int));
assert(q);
for (int i=0; i<n; i++) q[i] = i;
}
}
#pragma omp parallel for shared(p,q) default(none)
for (int i=0; i<n; i++)
p[i] += q[i];
for (int i=0; i<n; i++)
printf("%d", p[i]);
printf("\n");
#pragma omp parallel sections
{
#pragma omp section
{
free(p);
}
#pragma omp section
{
free(q);
}
}
}
