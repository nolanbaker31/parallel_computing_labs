/* nbody.c: sequential 2-d nbody simulation
   Author: Stephen Siegel

   Link this with a translation unit that defines the extern
   variables, and anim.o, to make a complete program.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <omp.h>
#include <cuda.h>
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
double x_span;                 /* x-length of window (meters) */
double y_span;                 /* y-length of window (meters) */

static inline double wrap(double x, const double min,
			  const double max, const double span) {
  while (x<min) x+=span;
  while (x>max) x-=span;
  return x;
}

static void init(char* filename) {
  #pragma omp parallel sections default (shared)
  {
    #pragma omp section
    {
  start_time = ANIM_time();
  assert(x_max > x_min && y_max > y_min);
  ny = ceil(nx*(y_max - y_min)/(x_max - x_min));
  x_span = x_max - x_min;
  y_span = y_max - y_min;
  printf("nbody: nbodies=%d nx=%d ny=%d nstep=%d wstep=%d\n",
	 nbodies, nx, ny, nstep, wstep);
  const int nframes =  wstep == 0 ? 0 : 1+nstep/wstep;
  printf("nbody: creating ANIM file %s with %d frames, %zu bytes.\n",
	 filename, nframes,
	 ANIM_Nbody_file_size_2d(nbodies, ncolors, nframes));
  fflush(stdout);
  assert(nx >= 10 && ny >= 10);
  assert(nstep >= 1 && wstep >= 0 && nbodies > 0);
  assert(ncolors >= 1 && ncolors <= ANIM_MAXCOLORS);
   }
  #pragma omp section
  {
  states = (State *)malloc(nbodies * sizeof(State));
  assert(states);
  states_new = (State *)malloc(nbodies * sizeof(State));
  assert(states_new);
  posbuf = (double *)malloc(2 * nbodies * sizeof(double));
  assert(posbuf);
  }}
  int radii[nbodies], bcolors[nbodies];
  ANIM_color_t acolors[ncolors]; // RGB colors converted to ANIM colors
  #pragma omp parallel for
  for (int i=0; i<nbodies; i++) {
    assert(bodies[i].mass > 0);
    assert(bodies[i].color >= 0 && bodies[i].color < ncolors);
    assert(bodies[i].radius > 0);
    states[i] = bodies[i].state;
    radii[i] = bodies[i].radius;
    bcolors[i] = bodies[i].color;
  }
  #pragma omp parallel for
  for (int i=0; i<ncolors; i++)
    acolors[i] = ANIM_Make_color(colors[i][0], colors[i][1], colors[i][2]);
  af =
    ANIM_Create_nbody_2d
    (nx, ny, x_min, x_max, y_min, y_max,
     nbodies, radii, ncolors, acolors, bcolors, filename);
}

static inline void write_frame() {
  for (int i=0, j=0; i<nbodies; i++) {
    posbuf[j++] = states[i].x;
    posbuf[j++] = states[i].y;
  }
  ANIM_Write_frame(af, posbuf);
}

/* Move forward one time step.  This is the "integration step".  For
   each body b, compute the total force acting on that body.  If you
   divide this by the mass of b, you get b's acceleration.  So you
   actually just calculate b's acceleration directly, since this is
   what you want to know.  Once you have the acceleration, update the
   velocity, then update the position. */
static inline void update() {
  for (int i=0; i<nbodies; i++) {
    double x = states[i].x, y = states[i].y;
    double vx = states[i].vx, vy = states[i].vy;
    // ax times delta t, ay times delta t...
    double ax_delta_t = 0.0, ay_delta_t = 0.0;

    for (int j=0; j<nbodies; j++) {
      if (j == i) continue;
      
      const double dx = states[j].x - x, dy = states[j].y - y;
      const double mass = bodies[j].mass;
      const double r_squared = dx*dx + dy*dy;
      
      if (r_squared != 0) {
	const double r = sqrt(r_squared);
	
	if (r != 0) {
	  const double acceleration = G * mass / r_squared;
	  const double atOverr = acceleration * delta_t / r;
	  
	  ax_delta_t += dx * atOverr;
	  ay_delta_t += dy * atOverr;
	}
      }
    }
    vx += ax_delta_t;
    vy += ay_delta_t;
    x += delta_t * vx;
    y += delta_t * vy;
    assert(!isnan(x) && !isnan(y) && !isnan(vx) && !isnan(vy));
    x = wrap(x, x_min, x_max, x_span);
    y = wrap(y, y_min, y_max, y_span);
    states_new[i] = (State){x, y, vx, vy};
  }
  State * const tmp = states; states = states_new; states_new = tmp;
}

/* Close GIF file, free all allocated data structures */
static void wrapup() {
  ANIM_Close(af);
  free(posbuf);
  free(states);
  free(states_new);
  printf("\nnbody: finished.  Time = %lf\n", ANIM_time() - start_time);
}

/* Two arguments: the name of the output file and num. threads */
int main(int argc, char * argv[]) {
  int statbar = 0; // used for printing status updates

  assert(argc == 3);
  init(argv[2]);
  if (wstep != 0) write_frame();
  for (int i=1; i<=nstep; i++) {
    update();
    ANIM_Status_update(stdout, nstep, i, &statbar);
    if (wstep != 0 && i%wstep == 0) write_frame();
  }
  wrapup();
}
