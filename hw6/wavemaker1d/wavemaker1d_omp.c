/* wavemaker1d.c: 1d-wave equation simulation, producing animation.
   Author: Stephen F. Siegel
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <omp.h>
#include "anim.h"
#define SQUARE(x) ((x)*(x))

int nthreads;
int nx;                   /* number of discrete points including endpoints */
int ny;                   /* number of pixels in y direction */
double k;                 /* physical constant to do with string */
int nstep;                /* number of time steps */
int wstep;                /* write frame every this many time steps */
ANIM_File af;             /* output file */
double start_time;        /* time simulation starts */
/* The following give the amplitude at each x-coordinate in
   3 consecutive time steps.  Each has length nx.  */
double * u_prev, * u_curr, * u_next; 

static void quit() {
  printf("Usage: wavemaker1d NX NY WIDTH K NSTEP WSTEP FILENAME       \n\
  NX = length (in pixels) in x direction, at least 5                  \n\
  NY = length (in pixels) in y direction, at least 5                  \n\
  WIDTH = width (in pixels) of initial pulse, in [2,NX-1]             \n\
  K = string elasticity, in [0.0,1.0]                                 \n\
  NSTEP = total number of time steps, at least 1                      \n\
  WSTEP = number of time steps between writes to file, [0,NSTEP]      \n\
  FILENAME = name of output file                                      \n\
If WSTEP=0 then no output is generated.  If WSTEP=NSTEP then only the \n\
initial (time 0) and final (time NSTEP) frames are generated.         \n\
Example: wavemaker1d 1000 600 300 0.005 50000 50 out.anim             \n");
  exit(1);
}

static void setup(int argc, char * argv[]) {
  if (argc != 8) quit();
  nx = atoi(argv[1]);
  ny = atoi(argv[2]);
  const int width = atoi(argv[3]);
  k = atof(argv[4]);
  nstep = atoi(argv[5]);
  wstep = atoi(argv[6]);
  char * const filename = argv[7];
  if (nx < 5 || ny < 5) quit();
  if (width < 2 || width >= nx) quit();
  if (k <= 0 || k > 1.0) quit();
  if (nstep < 1) quit();
  if (wstep < 0 || wstep > nstep) quit();
  printf("wavemaker1d: nx=%d ny=%d width=%d k=%.3lf nstep=%d wstep=%d nthreads=%d\n",
	 nx, ny, width, k, nstep, wstep, nthreads);
  const int nframes =  wstep == 0 ? 0 : 1+nstep/wstep;
  printf("wavemaker1d: creating ANIM file %s with %d frames, %zu bytes.\n",
	 filename, nframes, ANIM_Graph_file_size_1d(nx, nframes));
  fflush(stdout);
  start_time = ANIM_time();
  u_prev = (double*)malloc(nx*sizeof(double));
  u_curr = (double*)malloc(nx*sizeof(double));
  u_next = (double*)malloc(nx*sizeof(double));
  assert(u_prev); assert(u_curr); assert(u_next);
  const double e = exp(1.0), height = ny/2.1;
#pragma omp parallel for
  for (int i = 0; i < nx; i++)
    u_curr[i] = u_prev[i] =
      (i == 0 || i >= width ? 0.0 :
       height * e * exp(-1.0/(1-SQUARE(2.0*(i-width/2.0)/width))));
  af = ANIM_Create_graph_1d(nx, ny, 0.0, 1.0*nx, -ny/2.0, ny/2.0, filename);
}

static void teardown() {
  ANIM_Close(af);
  free(u_prev); free(u_curr); free(u_next);
  printf("\nwavemaker1d: finished.  Time = %lf\n", ANIM_time() - start_time);
}

void update() {
  #pragma omp parallel for schedule(static, nx/nthreads) 
  for (int i = 1; i < nx-1; i++)
    u_next[i] = 2.0*u_curr[i] - u_prev[i] +
      k*(u_curr[i+1] + u_curr[i-1] - 2.0*u_curr[i]);

  double * tmp = u_prev; u_prev = u_curr; u_curr = u_next; u_next = tmp;
}

int main(int argc, char *argv[]) {
  int dots = 0;
#pragma omp parallel
{
  nthreads = omp_get_num_threads();
}
  setup(argc, argv);
  if (wstep != 0) ANIM_Write_frame(af, u_curr);
  for (int i = 1; i <= nstep; i++) {
    update();
    ANIM_Status_update(stdout, nstep, i, &dots);
    if (wstep != 0 && i%wstep == 0)
      ANIM_Write_frame(af, u_curr);
  }
  teardown();
}
