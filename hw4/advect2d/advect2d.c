#include <stdlib.h>
#include <assert.h>
#include "anim.h"

const int n = 200;          // number of discrete points including endpoints
const double k = 0.01;      // D*dt/(dx*dx), diffusivity constant
const double c = 0.001;     // advection constant
const int nstep = 200000;   // number of time steps
const int wstep = 400;      // time between writes to file
const double m = 100.0;     // initial temperature of rod interior
char * filename = "advect2d.anim";  // name of file to create
double ** u, ** u_new;      // two copies of the temperature function
ANIM_File af;               // output file
double start_time;          // time simulation starts

static void setup(void) {
  int h0 = n/2-n/3, h1 = n/2+n/3;
  start_time = ANIM_time();
  printf("advect2d: n=%d k=%lf c=%lf nstep=%d wstep=%d\n",
	 n, k, c, nstep, wstep);
  u = ANIM_allocate2d(n,n);
  u_new = ANIM_allocate2d(n,n);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      u_new[i][j] = u[i][j] = 0.0;
  for (int i = h0; i < h1; i++)
    for (int j = h0; j < h1; j++)
      u_new[i][j] = u[i][j] = m;
  af = ANIM_Create_heat_2d(n, n, 0, 1, 0, 1, 0, m, filename);
}

static void teardown(void) {
  ANIM_Close(af);
  ANIM_free2d(u);
  ANIM_free2d(u_new);
  printf("\nTime (s) = %lf\n", ANIM_time() - start_time);
}

static void update() {
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      u_new[i][j] =  u[i][j] +
	k*(u[(i+1)%n][j]
	   + u[(i+n-1)%n][j] + u[i][(j+1)%n]
	   + u[i][(j+n-1)%n] - 4*u[i][j])
	- c*(u[(i+1)%n][j] - u[(i+n-1)%n][j]
	     + u[i][(j+1)%n] - u[i][(j+n-1)%n]);
  double ** const tmp = u_new; u_new = u; u = tmp;
}

int main() {
  int dots = 0;
  setup();
  if (wstep != 0) ANIM_Write_frame(af, u[0]);
  for (int i = 1; i <= nstep; i++) {
    update();
    ANIM_Status_update(stdout, nstep, i, &dots);
    if (wstep != 0 && i%wstep == 0) ANIM_Write_frame(af, u[0]);
  }
  teardown();
}
