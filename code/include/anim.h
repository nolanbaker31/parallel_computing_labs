#ifndef ANIM
#define ANIM

/* ANIM library, by Stephen F. Siegel, University of Delaware, 2020,
   2022.

   ANIM is a simple C library for creating animations from scientific
   programs.  The following kinds of animations are supported:

   HEAT: a real-valued function is defined at each point in a 1d, 2d,
   or 3d spatial domain.  The value of the function changes with time.
   At any point in time, the values of the function are displayed by
   pixel color, where the colors lie on a blue-red spectrum, with blue
   representing the lowest value, and red the highest.

   GRAPH: a real-valued function is defined at each point in a 1d or
   2d spatial domain.  The function changes with time.  At each point
   in time, the values are displayed as a graph, i.e, as a set of
   points (x,f(x)) in the x-y plane for the 1d case, or as a set of
   points (x,y,f(x,y)) in the x-y-z space in the 2d case.  Note the
   dimension of the graph itself is one higher than the dimension of
   its domain.

   NBODY: some fixed number of bodies is specified.  Each body has a
   fixed color and radius, and is represented as a solid sphere of
   that radius and color.  The dimension of the spatial domain can be
   1, 2, or 3.  The bodies move with time.  At each point in time, the
   position of each body is specified and the bodies are displayed at
   those points.

   An animation is created by one of the ANIM_Create* functions, which
   creates a new file and returns a handle to it.  Frames are added to
   this file by function ANIM_Write_frame.  The file is closed with
   ANIM_Close.  The file created is in a binary format called "ANIM
   format" and usually ends with suffix .anim.  Separate utilities can
   be used to transform the file to an animated GIF (.gif) or MPEG
   movie (.mp4).
*/
#include <stdio.h>
#include <limits.h>

/* The maximum length, in pixels, of an image in any dimension.  The
   maximum coordinate in any dimension is one less than this number,
   since the coordinates are numbered from 0.  */
#define ANIM_LENGTH_MAX (USHRT_MAX - 1)

/* A number that can be used for a coordinate that represents no
   value, or undefined */
#define ANIM_COORD_NULL (USHRT_MAX - 1)

/* Maximum number of distinct colors that can be used in an animation */
#define ANIM_MAXCOLORS 256


/* An ANIM_File is an opaque handle to an ANIM File object.  The
   object can be read or modified only through this handle, using the
   functions in this header file. */
typedef struct ANIM_File_s * ANIM_File;

/* Enumeration of the different kinds of animations */
typedef enum { ANIM_HEAT, ANIM_GRAPH, ANIM_NBODY } ANIM_Kind;

/* A range specifies an upper and lower bound for a real interval */
typedef struct ANIM_range_s {
  double min;
  double max;
} ANIM_range_t;

/* A color */
typedef unsigned int ANIM_color_t;

/* Integer in range 0..255 */
typedef unsigned char ANIM_byte;

/* Turns the debugging flag on.  Messages will be printed to stdout.
   Should only be used for small problem sizes. */
void ANIM_Set_debug();

/* Turns the debugging flag off */
void ANIM_Unset_debug();

/* Make a color from red, green, blue intensity */
ANIM_color_t ANIM_Make_color(ANIM_byte red, ANIM_byte green, ANIM_byte blue);


/* Starts a new 1d-animation of the HEAT kind.
   
   length: number of pixels in x-direction
   x_min: minimum x-coordinate of domain
   x_max: maximum x-coordinate of domain
   value_min: lower bound on values taken on by function
   value_max: upper bound on values taken on by function
   filename: name of file to create, where data will be written
 */
ANIM_File ANIM_Create_heat_1d(int length, double x_min, double x_max,
			      double value_min, double value_max,
			      char * filename);
/* Starts a new 2d-animation of the HEAT kind.
   
   x_length: number of pixels in x-direction
   y_length: number of pixels in y-direction
   x_min: minimum x-coordinate of domain
   x_max: maximum x-coordinate of domain
   y_min: minimum y-coordinate of domain
   y_max: maximum y-coordinate of domain
   value_min: lower bound on values taken on by function
   value_max: upper bound on values taken on by function
   filename: name of file to create, where data will be written
 */
ANIM_File ANIM_Create_heat_2d(int x_length, int y_length,
			      double x_min, double x_max,
			      double y_min, double y_max,
			      double value_min, double value_max,
			      char * filename);

/* Starts a new animation of the GRAPH kind in which the spatial
   domain is one-dimensional.  This is used to represent a function
   that takes one real input and returns one real output.  The graph
   image will be 2-dimensional, with the x-axis representing the domain,
   and the y-axis representing the values taken on by the function,
   just as in a typical math book.

   x_length: number of pixels in x-direction
   y_length: number of pixels in y-direction
   x_min: minimum x-coordinate of domain
   x_max: maximum x-coordinate of domain
   y_min: lower bound on value taken on by function
   y_max: upper bound on value taken on by function
   filename: name of file to create, where data will be written
*/
ANIM_File ANIM_Create_graph_1d(int x_length, int y_length,
			       double x_min, double x_max,
			       double y_min, double y_max,
			       char * filename);

/* Starts a new 1d-animation of the NBODY kind.

   length: number of pixels in x-direction
   x_min: minimum x-coordinate of spatial domain
   x_max: maximum x-coordinate of spatial domain
   nbodies: number of bodies (fixed for duration of animation)
   radii: radius of each body, in pixels
   ncolors: the number of different colors that will be used for bodies
   colors: the ncolors colors
   bcolors: the color of each body, specified as an index into colors
   filename: name of file to create, where data will be written
 */
ANIM_File ANIM_Create_nbody_1d(int length, double x_min, double x_max,
			       int nbodies, int radii[],
			       int ncolors,  ANIM_color_t colors[],
			       int bcolors[], char * filename);

/* Starts a new 2d-animation of the NBODY kind.

   x_length: number of pixels in x-direction
   y_length: number of pixels in y-direction
   x_min: minimum x-coordinate of spatial domain
   x_max: maximum x-coordinate of spatial domain
   y_min: minimum y-coordinate of spatial domain
   y_max: maximum y-coordinate of spatial domain
   nbodies: number of bodies (fixed for duration of animation)
   radii: radius of each body, in pixels
   ncolors: the number of different colors that will be used for bodies
   colors: the ncolors colors
   bcolors: the color of each body, specified as an index into colors
   filename: name of file to create, where data will be written
 */
ANIM_File ANIM_Create_nbody_2d(int x_length, int y_length,
			       double x_min, double x_max,
			       double y_min, double y_max,
			       int nbodies, int radii[],
			       int ncolors,  ANIM_color_t colors[],
			       int bcolors[], char * filename);

/* Adds a frame to the ANIM file af.  File af must be previously
   created by one of the ANIM_Create* functions, and not yet closed.

   buf must point to a buffer that holds at least n doubles, where
   n is the frame size.  This is defined as follows:

   For HEAT and GRAPH animations, n = lengths[0] * ... * lengths[dim-1].
   This is the total number of discrete points in the spatial domain,
   which is the domain of the function.

   For HEAT and GRAPH, values outside of the specified range will
   result in an error message.

   For NBODY animations, n = nbodies*dim.   This provides one point
   in dim dimension space for each body.  The buf consists of
   a sequence of nbodies points, x0, y0, x1, y1, .... (for dim = 2).
   Points outside of the spatial domain are allowed but will appear
   fully or partially offscreen.
   
   The data in buf is written to the file in a compressed format which
   loses precision.

   Returns the total number of elements written, which should equal
   the value n defined above.  Any other return value indicates an
   error. */
size_t ANIM_Write_frame(ANIM_File af, double * buf);


/* Computes the size of an ANIM file, in bytes, for a 1d-heat
   simulation with nframes frames */
size_t ANIM_Heat_file_size_1d(int length, size_t nframes);

/* Computes the size of an ANIM file, in bytes, for a 2d-heat
   simulation with nframes frames */
size_t ANIM_Heat_file_size_2d(int x_length, int y_length,
			      size_t nframes);

/* Computes the size of an ANIM file, in bytes, for an ANIM_GRAPH
   with a 1d-domain and nframes frames */
size_t ANIM_Graph_file_size_1d(int x_length, size_t nframes);

/* Computes the size of an ANIM file, in bytes, for a 1d ANIM_NBODY
   animation with nbodies bodies, ncolors colors, and nframes frames */
size_t ANIM_Nbody_file_size_1d(int nbodies, int ncolors, size_t nframes);

/* Computes the size of an ANIM file, in bytes, for a 2d ANIM_NBODY
   animation with nbodies bodies, ncolors colors, and nframes frames */
size_t ANIM_Nbody_file_size_2d(int nbodies, int ncolors, size_t nframes);

/* Opens an existing ANIM file for reading.  The file must have been
   perviously created with one of the ANIM_Create* functions, and
   subsequently closed.  Returns an opaque handle to the file.  Only
   the file header is read by this function.  To read the data, make
   repeated calls to ANIM_Read.  */
ANIM_File ANIM_Open(char * filename);

/* Reads the next frame from the ANIM file af, if there is one.  The
   ANIM File af must be open for reading.  The values read are
   converted to doubles, i.e., to the same type used to write them.
   Note that precision will be lost, because the intermediate form
   used to store the data in an ANIM file has much less precision than
   double.

   Returns the total number of values read.  This should be either 0
   (indicating end of file) or the frame size of af.  The return of
   any other number indicates an error.

   If the number of values read is not 0, the framecounter of af is
   incremented.  The values read from af are written to buf, which
   must point to a region of memory capable of holding that many
   doubles. */
size_t ANIM_Read_frame(ANIM_File af, double * buf);

/* Gets the dim (spatial dimension) of the ANIM file.  The ANIM file
   af must be open (for reading or for writing). */
int ANIM_Get_dim(ANIM_File af);

/* Gets the length of a frame in the given dimension.  index is an
   integer in the range [0, dim-1] for a HEAT or NBODY animation; it
   is in [0,dim] for a GRAPH animation. */
int ANIM_Get_image_length(ANIM_File af, int index);

/* Number n of scalar values in one frame.

   For HEAT and GRAPH animations, n = pixels[0] * ... * pixels[dim-1].
   This is the total number of discrete points in the spatial domain,
   which is the domain of the function.

   For NBODY animations, n = nbodies*dim.   This provides one point
   in dim dimension space for each body.
 */
size_t ANIM_Get_frame_size(ANIM_File af);

/* Returns the range of values represented by the index-th dimension.
   For HEAT and GRAPH animations, index is in [0, dim], the final
   range in position dim representing the range of values taken on by
   the real function.  For NBODY animations, index is in [0,dim-1]. */
ANIM_range_t ANIM_Get_range(ANIM_File af, int index);

/* Returns the file name */
char * ANIM_Get_filename(ANIM_File af);

/* Returns the current value of framecount.  The ANIM file af must be
   open for reading or writing.  This will be the number of frames
   read or the number of frames written. */
size_t ANIM_Get_framecount(ANIM_File af);

/* Returns the kind of the ANIM file */
ANIM_Kind ANIM_Get_kind(ANIM_File af);

/* Closes an open ANIM file, whether it was opened for reading or
   created for writing.  The underlying ANIM_File object is destroyed,
   and the handle af should not be used again. */
void ANIM_Close(ANIM_File af);

void ANIM_Print_metadata(FILE * out, ANIM_File af);

/* Reads the ANIM file with the given filname and prints its contents
   to stdout in a human readable format, as floating point numbers
   with low precision.  Useful mainly for debugging. */
void ANIM_Print(char * filename);

/* Returns the current calendar time, in second, from some fixed point
   in the past. */
double ANIM_time();

/* Prints a simple status bar.  Assumes there is some known, fixed
   number of steps (nsteps) for a process.  Call this function at any
   time, e.g., every time a step completes.  You must pass it a
   pointer to a variable of type size_t, which should be initially 0.
   That variable is used to store the number of characters already
   printed. */
void ANIM_Status_update(FILE * stream, size_t nsteps, size_t step,
			int * statvar);

/* Allocates a 2d array of doubles of size n x m. */
double ** ANIM_allocate2d(int n, int m);

/* Dellocates a 2d array that was created by ANIM_allocate2d */
void ANIM_free2d(double ** a);

#endif
