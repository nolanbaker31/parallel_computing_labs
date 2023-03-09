/* nbody.c: MPI/OpenMP 2-d nbody simulation
   Author: Stephen Siegel

   Link this with a translation unit that defines the extern
   variables, and anim.o, to make a complete program.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <mpi.h>
#include <pthread.h>
#include "nbody.h"
#include "anim.h"

/* Global variables */
extern const double x_min;     /* coord of left edge of window (meters) */
extern const double x_max;     /* coord of right edge of window (meters) */
extern const double y_min;     /* coord of bottom edge of window (meters) */
extern const double y_max;     /* coord of top edge of window (meters) */
extern const int nx;           /* width of movie window (pixels) */
extern const int nbodies;      /* number of bodies */
extern const double delta_t;   /* time between discrete time steps (seconds) */
extern const int nstep;        /* number of time steps */
extern const int wstep;        /* number of times steps beween movie frames */
extern const int ncolors;      /* number of colors to use for the bodies */
extern const int colors[][3];  /* colors we will use for the bodies */
extern const Body bodies[];    /* list of bodies with initial data */
const double G = 6.674e-11;    /* universal gravitational constant */
int ny;                        /* height of movie window (pixels) */
State * states, * states_new;  /* two copies of state array */
ANIM_File af;                  /* output anim file */
double * posbuf;               /* to send data to anim, 2*nbodies doubles */
double start_time;             /* time simulation starts */
int nprocs;
int rank;
int nthreads; // number of threads per proc

void * f(void * arg) {
  int tid = *(int*)arg;

  printf("Proc %d: Pthread %d\n", rank, tid);
  return NULL;
}

/* Two arguments: the name of the output file and number of threads */
int main(int argc, char * argv[]) {
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    assert(argc == 3);
    nthreads = atoi(argv[2]);
    printf("nbodies=%d nstep=%d wstep=%d nprocs=%d nthreads=%d\n",
	   nbodies, nstep, wstep, nprocs, nthreads);
  }
  MPI_Bcast(&nthreads, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    int statbar = 0; // used for printing status updates

    printf("G = %E\n", G);
    for (int i=1; i<=nstep; i++) {
      ANIM_Status_update(stdout, nstep, i, &statbar);
    }
    printf("\n");
  }

  int tids[nthreads];
  pthread_t threads[nthreads];

  for (int i=0; i<nthreads; i++) {
    tids[i] = i;
    pthread_create(&threads[i], NULL, f, &tids[i]);
  }
  for (int i=0; i<nthreads; i++)
    pthread_join(threads[i], NULL);
  MPI_Finalize();
}
