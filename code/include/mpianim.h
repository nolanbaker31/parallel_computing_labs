#ifndef _MPIANIM
#define _MPIANIM
/* MPIANIM library, by Stephen F. Siegel, University of Delaware, 2020.

   This an MPI-based parallel version of the ANIM library.  It
   provides the basic functions needed to construct an ANIM animation
   from a C/MPI programs.  The ANIM file produced will be in the exact
   same format as that produced by a sequential program, and can be
   manipulated with all the same tools (anim2txt, anim2gif, anim2mp4,
   etc).

   All of the functions here are collective: they must be called by
   every process in the communicator that is carrying out the
   simulation. */
#include <stdio.h>
#include <limits.h>
#include <mpi.h>
#include "anim.h"

/* Opaque handle for the MPIANIM object representing the file under
   construction. */
typedef struct MPIANIM_File_s * MPIANIM_File;

/* Create a heat map simulation.  Collective function.  Each process
   specifies the part of the domain that it is responsible for.

     dim: dimension of spatial domain (1, 2, or 3).  Same on all procs.
     lengths: length of domain in pixels in each dimension.  Same on all procs.
     ranges: real intervals for domain and function values
     sublengths: lengths of the sides of rectangular region for this proc
     starts: point in the domain which is the lower corner for this proc
     filename: name of file to create (same on all procs)
     comm: communicator specifying set of procs taking part (same on all procs)

   Note: each proc defines a rectangular prism with one corner starts
   and side lengths sublengths.  The set of these rectangular prisms
   over all procs must partition the full domain, else behavior is
   undefined.

   Example: the following partitions the nx x ny global domain into
   vertical stripes, one stripe for each process.  Process rank is
   responsible for all x-coordinates from first_x to first_x+nxl-1.
   Note that lengths={nx,ny} are the global dimensions.  Argument
   sublengths = {nxl,ny} gives the dimensions of the stripe belonging
   to this process.  starts={first_x,0} is the coordinate of the lower
   left corner of the stripe owned by this proc.  The choice of ranges
   specifies that the x-axis will be labeled from 0 to 1, and same for
   the y-axis (this data is currently not used).  The value taken on
   by the function at any point must be a double in the range
   [0,nprocs-1].

   nxl = nx/nprocs;
   first_x = rank*nxl;
   if (rank == nprocs - 1) nxl += nx%nprocs;
   af = MPIANIM_Create_heat(2, (int[]){nx, ny},
                            (ANIM_range_t[]){{0, 1}, {0, 1}, {0, nprocs-1}},
                            (int[]){nxl, ny}, (int[]){first_x, 0}, filename,
                            MPI_COMM_WORLD);
 */
/*
MPIANIM_File MPIANIM_Create_heat(int dim,
				 int lengths[dim],
				 ANIM_range_t ranges[dim+1],			   
				 int sublengths[dim],
				 int starts[dim],
				 char * filename,
				 MPI_Comm comm);
*/


/* Create a 1d heat map simulation.  Collective function.  Each process
   specifies the part of the domain that it is responsible for.

   length: length of x-axis in pixels; same on all procs
   x_min: minimum x-coordinate of domain; same on all procs
   x_max: maximum x-coordinate of domain; same on all procs
   value_min: lower bound on values taken on by function; same on all procs
   value_max: upper bound on values taken on by function; same on all procs
   sublength: length in pixels of the sub-interval for this proc
   start: left point (in pixels) for this proc's interval
   filename: name of file to create (same on all procs)
   comm: communicator specifying set of procs taking part (same on all procs)
*/
MPIANIM_File MPIANIM_Create_heat_1d(int length,
				    double x_min, double x_max,
				    double value_min, double value_max,
				    int sublength,
				    int start,
				    char * filename,
				    MPI_Comm comm);

/* Create a 2d heat map simulation.  Collective function.  Each process
   specifies the part of the domain that it is responsible for.

   x_length: number of pixels in x-direction; same on all procs
   y_length: number of pixels in y-direction; same on all procs
   x_min: minimum x-coordinate of domain; same on all procs
   x_max: maximum x-coordinate of domain; same on all procs
   y_min: minimum y-coordinate of domain; same on all procs
   y_max: maximum y-coordinate of domain; same on all procs
   value_min: lower bound on values taken on by function; same on all procs
   value_max: upper bound on values taken on by function; same on all procs
   x_sublength: length of subinterval's x direction for this proc
   y_sublength: length of subinterval's y direction for this proc
   x_start: starting point in x direction for this proc's subinterval
   y_start: starting point in y direction for this proc's subinterval
   filename: name of file to create; same on all procs
   comm: communicator specifying set of procs taking part; same on all procs

   Note: each proc defines a rectangular prism with one corner starts
   and side lengths sublengths.  The set of these rectangular prisms
   over all procs must partition the full domain, else behavior is
   undefined.

   Example: the following partitions the nx x ny global domain into
   vertical stripes, one stripe for each process.  Process rank is
   responsible for all x-coordinates from first_x to first_x+nxl-1.
   The global dimensions are nx x ny.  The dimensions of the stripe
   belonging to this process are nxl x ny.  The coordinate of the
   lower left corner of the stripe owned by this proc is (first_x, 0).
   The choice of ranges specifies that the x-axis will be labeled from
   0 to 1, and same for the y-axis (this data is currently not used).
   The value taken on by the function at any point must be a double in
   the range [0,nprocs-1].

   nxl = nx/nprocs;
   first_x = rank*nxl;
   if (rank == nprocs - 1) nxl += nx%nprocs;
   af = MPIANIM_Create_heat_2d(nx, ny, 0, 1, 0, 1, 0, nprocs-1, nxl, ny,
                               first_x, 0, filename, MPI_COMM_WORLD);
 */
MPIANIM_File MPIANIM_Create_heat_2d(int x_length, int y_length,
				    double x_min, double x_max,
				    double y_min, double y_max,
				    double value_min, double value_max,
				    int x_sublength, int y_sublength,
				    int x_start, int y_start,
				    char * filename,
				    MPI_Comm comm);

/* Creates a GRAPH animation.  Similar to Create_heat above. */
/*
MPIANIM_File MPIANIM_Create_graph(int dim,
				  int lengths[dim+1],
				  ANIM_range_t ranges[dim+1], 
				  int sublengths[dim],
				  int starts[dim],
				  char * filename,
				  MPI_Comm comm);
*/

MPIANIM_File MPIANIM_Create_graph_1d(int x_length, int y_length,
				     double x_min, double x_max,
				     double y_min, double y_max,
				     int sublength, int start,
				     char * filename,
				     MPI_Comm comm);



/* Creates an NBODY animation.  Collective function.  Each process is
   responsible for a set of bodies.  The set of bodies belonging to a
   process form a consecutive block in the global array.  The block is
   specified by the index of the first element owned by the proc, and
   the number of elements in the block.  These blocks must partition
   the global array of bodies.
 */
/*
MPIANIM_File MPIANIM_Create_nbody(int dim,
				  int lengths[dim],
				  ANIM_range_t ranges[dim],
				  int ncolors, 
				  ANIM_color_t colors[ncolors], 
				  int nbodies,
				  int blocklength,
				  int first,
				  int radii[blocklength],
				  int bcolors[blocklength],
				  char * filename,
				  MPI_Comm comm);
*/


MPIANIM_File MPIANIM_Create_nbody_1d(int length,
				     double x_min, double x_max,
				     int ncolors, 
				     ANIM_color_t colors[ncolors], 
				     int nbodies,
				     int blocklength,
				     int first,
				     int radii[blocklength],
				     int bcolors[blocklength],
				     char * filename,
				     MPI_Comm comm);

MPIANIM_File MPIANIM_Create_nbody_2d(int x_length, int y_length,
				     double x_min, double x_max,
				     double y_min, double y_max,
				     int ncolors, 
				     ANIM_color_t colors[ncolors], 
				     int nbodies,
				     int blocklength,
				     int first,
				     int radii[blocklength],
				     int bcolors[blocklength],
				     char * filename,
				     MPI_Comm comm);

/* Extends the ANIM file to be large enough to hold nframes frames.
   Use of this function is optional.  It may be called directly after
   one of the MPIANIM_Create functions to specify the number of frames
   that will be written to the file.  This gives the IO system an
   opportunity to optimize the file creation process.  Everything will
   work fine if this function is never called, but using this function
   may speed up the IO.

   If more than nframes frames are written, there is no
   problem---everything will still work correctly.  If fewer than
   nframes frames are written, there will be extra frames containing
   uninitialized data in the file.  If this function is called after
   one or more frames have already been written, there is no problem
   if nframes is at least the number already written; if nframes is
   less than the number already written, then some frames may be
   lost. */
void MPIANIM_Set_nframes(MPIANIM_File af, size_t nframes);

/* Write a new frame to the animation.  Collective function.  Each
   process contributes the data for its part of the problem.  For HEAT
   and GRAPH animations, this means the values on the rectangular
   portion of the domain owned by this process.  The data must occur
   in the order used by ANIM_Write_frame: first all values for the
   column in the first x-coordinate, then all the values for the
   column in the second x-coordinate, etc.

   Example: the following code allocates a buffer of doubles of size
   nxl * ny.  The process fills that buffer with rank at every point.

   buf = malloc(nxl * ny * sizeof(double));
   assert(buf);
   for (int i=0; i<nxl; i++)
     for (int j=0; j<ny; j++)
       buf[i*ny + j] = rank;
   MPIANIM_Write_frame(af, buf, MPI_STATUS_IGNORE);

   For NBODY, "the data for its part of the problem" means the
   positions of the bodies belonging to the block of bodies owned by
   this process.

   The status returns the number of scalars successfully written to
   the file, which should be the same as the number of scalars for
   which this process is responsible, unless something goes wrong with
   the I/O.  */
void MPIANIM_Write_frame(MPIANIM_File af, double * buf, MPI_Status * status);

/* Complete and close the animation.  Collective function */
void MPIANIM_Close(MPIANIM_File af);

#endif
