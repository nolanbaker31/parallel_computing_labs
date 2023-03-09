#include <stdlib.h>
#include <assert.h>
#include <mpi.h>
#include "mpianim.h"

const int n = 800;         // number of discrete points including endpoints
const double k = 0.01;     // D*dt/(dx*dx), diffusivity constant
const double c = 0.001;    // advection constant
const int nstep = 300000;  // number of time steps
const int wstep = 400;     // time between writes to file
const double m = 100.0;    // initial temperature of interior
char * filename = "advect2d_mpi.anim";  // name of file to create
double ** u, ** u_new;     // two copies of the temperature function
MPIANIM_File af;           // output file
double start_time;         // time simulation starts
int nprocs, rank;
int first, nl;
int left, right;
int mean, average;
int min = n;
int local_min;
int sum = 0;

static void setup(void) {
  local_min = n;
  int h0 = n/2-n/3, h1 = n/2+n/3;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  first = rank*n/nprocs;
  nl = (rank+1)*n/nprocs - first;
  start_time = ANIM_time();
  u = ANIM_allocate2d(nl+2,n);
  u_new = ANIM_allocate2d(nl+2,n);
  for (int i = 0; i < nl+2; i++)
    for (int j = 0; j < n; j++)
      u_new[i][j] = u[i][j] = 0.0;

  for(int i = 0; i < nl+2; i++){
    for(int j = h0; j < h1; j++){
	  if((i+first > h0) && (i+first < h1)){
	  u_new[i][j] = u[i][j] = m;
	  }}}

  af = MPIANIM_Create_heat_2d
    (n, n, 0, 1, 0, 1, 0, m,
     nl, n, first, 0, filename, MPI_COMM_WORLD);
}

static void teardown(void) {
  MPIANIM_Close(af);
  ANIM_free2d(u);
  ANIM_free2d(u_new);
  if (rank == 0)
    printf("\n");
}

static void exchange() { //left ghost cell tag = 0, right ghost cell tag = 1
	if(rank == 0){
		MPI_Sendrecv(&(u[1][0]), n, MPI_DOUBLE, nprocs-1, 1, &(u[nl+1][0]), n, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Sendrecv(&(u[nl][0]), n, MPI_DOUBLE, rank+1, 0, &(u[0][0]), n, MPI_DOUBLE, nprocs-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	else if(rank == (nprocs-1)){
		MPI_Sendrecv(&(u[1][0]), n, MPI_DOUBLE, rank-1, 1, &(u[nl+1][0]), n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Sendrecv(&(u[nl][0]), n, MPI_DOUBLE, 0, 0, &(u[0][0]), n, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	else{
		MPI_Sendrecv(&(u[1][0]), n, MPI_DOUBLE, rank-1, 1, &(u[nl+1][0]), n, MPI_DOUBLE, rank+1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Sendrecv(&(u[nl][0]), n, MPI_DOUBLE, rank+1, 0, &(u[0][0]), n, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	//printf("rank %d exchanged!\n", rank);
	//fflush(stdout);

}

static void update() {
  for (int i = 1; i <= nl; i++)
    for (int j = 0; j < n; j++)
      u_new[i][j] =  u[i][j] + k*(u[(i+1)%n][j] + u[(i+n-1)%n][j] + u[i][(j+1)%n] + u[i][(j+n-1)%n] - 4*u[i][j]) - c*(u[(i+1)%n][j] - u[(i+n-1)%n][j] + u[i][(j+1)%n] - u[i][(j+n-1)%n]);
  double ** const tmp = u_new; u_new = u; u = tmp;
}

int main() {
  int dots = 0;
  MPI_Init(NULL, NULL);
  setup();
  if (wstep != 0)
    MPIANIM_Write_frame(af, &u[1][0], MPI_STATUS_IGNORE);
  for (int i = 1; i <= nstep; i++) {
    exchange();
    update();
    if (rank == 0)
      ANIM_Status_update(stdout, nstep, i, &dots);
    if (wstep != 0 && i%wstep == 0)
      MPIANIM_Write_frame(af, &u[1][0], MPI_STATUS_IGNORE);
 }
  for(int i = 1; i <= nl+1; i++){
	  for(int j = 0; j<n; j++){
	  sum+=u[i][j];
		  if(u[i][j] < min)
			 local_min = u[i][j];
	  }
  }
  MPI_Reduce(&sum, &average, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&local_min, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
  if(rank == 0){
  average = average / (n*n);
  printf("Mean temperature is: %d, Minimum temperature is: %d", average, min);
  }
  teardown();
  MPI_Finalize();
}
