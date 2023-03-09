#ifndef ANIM_DEV
#define ANIM_DEV
/* anim_dev.h: Header file revealing some of the implementation details
   of the ANIM library.  To be used internally, by other ANIM tools.
 */
#include "anim.h"

/*  The ANIM_File structure definition.  Not every field is used for
    every kind. */
struct ANIM_File_s {
  /* What kind of animation is this? */
  ANIM_Kind kind;

  /* Are we reading (not writing) this file? */
  bool read;

  /* The dimension of the spatial domain.  1, 2, or 3. */
  int dim;

  /* Length of each dimension of the image, in pixels. For HEAT and
     NBODY, this is an array of length dim.  For GRAPH, length
     dim+1.   Extra cells are ignored. */
  int lengths[3];

  /* The intervals defining the domain and range of the function.  For
     HEAT and GRAPH, this is an array of length dim+1.  For NBODY, the
     length is dim. */
  ANIM_range_t ranges[4];

  /* The number of frames in the final animation, set at very end of
     writing, when file is closed. */
  size_t nframes;
  
  /* The total number of scalar elements in one frame of the
     animation.  For HEAT and GRAPH, this is lengths[0] * ... *
     lengths[dim-1].  For NBODY, it is nbodies*dim. */
  size_t frame_size; // the frame size, #scalars

  /* The number of frames written or read so far: this is updated
     at each read or write of a frame */
  size_t framecount;

  /* Name of the underlying file */
  char * filename;

  /* The stream used to access the file */
  FILE * file;

  /* Buffer holding compressed data for one frame.  For HEAT, buf is a
   sequence of ANIM_byte_t.  For GRAPH and NBODY, buf is a sequence of
   ANIM_coord_t. */
  void * buf;

  // The following fields used for NBODY kind only...

  /* The number of bodies */
  int nbodies;

  /* The radius of each body.   Array of length nbodies. */
  int * radii;

  /* The number of different colors used for the bodies.  This has to
     be small, like at most 255, due to limitations in the GIF
     format. */
  int ncolors;

  /* The different colors used.  Array of length ncolors. */
  ANIM_color_t * colors;

  /* The color of each body.  Array of length bcolors.  The color of
     body i is colors[bcolors[i]]. */
  int * bcolors;
};

/* The type used to store a coordinate in an ANIM file */
typedef unsigned short ANIM_coord_t;

/* Converts a kind code to a string */
char * ANIM_kind_to_str(ANIM_Kind kind);

/* Converts a kind code to an int */
int ANIM_kind_to_int(ANIM_Kind kind);

/* Converts an int back to kind */
int ANIM_int_to_kind(int val);

/* Gives the number of bytes in the ANIM file header from the
   beginning of the file to the point in the file that stores the
   nframes field.  */
long int ANIM_Get_nframes_offset();

/* Reads the next frame (if there is one) from the open file into the
   internal buffer of af.  Returns the number of values read, exactly
   as in ANIM_Read_frame().  If the number is not zero, the frame
   counter of af is incremented.

   The values read are in the internal format of ANIM --- they are not
   converted into doubles.  This function should only be used by
   experts.
*/
size_t ANIM_Read_next(ANIM_File af);

/* Sets the current position of the file pointer to the specified
   frame.  Should be used only when reading, not writing.  The frames
   are numbered from 0.  If frame > af->nframes, behavior is
   undefined. */
void ANIM_Seek_frame(ANIM_File af, size_t frame);

/* Returns the pointer to the internal buffer of af, used to store
   bytes read from the file.  Only for experts. */
void * ANIM_Get_buffer(ANIM_File af);

/* Gets the red intensity of the color */
ANIM_byte ANIM_Get_red(ANIM_color_t color);

/* Gets the green intensity of the color */
ANIM_byte ANIM_Get_green(ANIM_color_t color);

/* Gets the blue intensity of the color */
ANIM_byte ANIM_Get_blue(ANIM_color_t color);

/* Prints an array of ints */
void ANIM_Print_int_array(FILE * out, int n, int * a);

/* Prints an array of ranges */
void ANIM_Print_range_array(FILE * out, int n, ANIM_range_t * a);

// inlined functions...

/* Converts a double to a byte, given fixed upper and lower bounds
   on the range of doubles */
static inline ANIM_byte
ANIM_double_to_byte(double temp, double min, double max) {
  if (temp < min || temp > max) {
    fprintf(stderr, "ANIM: value %lf not in interval [%lf, %lf]\n",
	    temp, min, max);
    fflush(stderr);
    exit(1);
  }
  
  unsigned int shade =
    (unsigned int)((temp - min)*ANIM_MAXCOLORS/(max - min));

  if (shade >= ANIM_MAXCOLORS) shade = ANIM_MAXCOLORS-1;
  return (ANIM_byte)shade;
}

/* Converts a byte to a double, given the fixed upper and lower bounds
   on the range of doubles */
static inline double
ANIM_byte_to_double(ANIM_byte shade, double min, double max) {
  return ((double)shade)*(max - min)/ANIM_MAXCOLORS + min;
}

/* Converts a double to a coord (16 bit precision). */
static inline ANIM_coord_t
ANIM_double_to_coord(double val, int n, double min, double max) {
  // Don't necessarily want to quit if coordinate is out of range,
  // because you can still draw it "off frame"...
  /*
  if (val < min || val > max) {
    fprintf(stderr, "ANIM: value %lf not in interval [%lf, %lf]\n",
	    val, min, max);
    fflush(stderr);
    exit(1);
  }
  */
  // pixels go from 0 to n-1
  // assuming min<=val<=max convert to integer range [0,n-1]
  // 0.0 <= val-min <= max-min
  // 0.0 <= (val-min)*(n-1.0)/(max-min) <= n-1.0
  // 0.5 <= (val-min)*(n-1.0)/(max-min) + 0.5 <= n-0.5
  // 0   <= (int)((val-min)*(n-1.0)/(max-min) + 0.5) <= n-1
  const ANIM_coord_t coord =
    (ANIM_coord_t) ((val - min)*(n - 1.0)/(max - min) + 0.5);
  // assert(0 <= coord && coord <= n-1);
  return coord;
}

/* Converts a coord back to a double */
static inline double
ANIM_coord_to_double(ANIM_coord_t coord, int n, double min, double max) {
  // Allowing out of range coordinates because they can be drawn "off frame"...
  // assert(0 <= coord && coord <= n-1);

  // the following was:
  //  const double result = ((double)(n - 1 - coord)) * (max - min)/(n - 1) + min;
  // but we don't want to invert in the conversion.
  const double result = ((double)coord) * (max - min)/(n - 1) + min;
  // assert(min <= result && result <= max);
  return result;
}

/* Starts a new animation of the HEAT kind.
   
   dim: the dimension of the spatial domain (1, 2, or 3)
   lengths: number of pixels in each dimension; array of length dim
   ranges: the range of values represented by each axis in the spatial domain,
     plus one more range for the range of values taken on by the function
   filename: name of file to create, where data will be written 
*/
ANIM_File ANIM_Create_heat(int dim, int lengths[dim],
			   ANIM_range_t ranges[dim+1],
			   char * filename); 

/* Computes the size of an ANIM file, in bytes, with kind ANIM_HEAT
   and nframes frames */
size_t ANIM_Heat_file_size(int dim, int lengths[dim], size_t nframes);

/* Starts a new animation of the GRAPH kind.
   
   dim: the dimension of the spatial domain (1 or 2)
   lengths: number of pixels in each dimension of the domain, plus one more
           for the range
   ranges: the range of values represented by each axis in the spatial domain,
           plus one more range for the range of values taken on by the function
   filename: name of file to create, where data will be written
*/
ANIM_File ANIM_Create_graph(int dim, int lengths[dim+1],
			    ANIM_range_t ranges[dim+1],
			    char * filename);


/* Computes the size of an ANIM file, in bytes, with kind ANIM_GRAPH
   and nframes frames */
size_t ANIM_Graph_file_size(int dim, int lengths[dim], size_t nframes);


/* Starts a new animation of the NBODY kind.
   
   dim: the dimension of the spatial domain (1, 2, or 3)
   lengths: number of pixels in each dimension; array of length dim
   ranges: the range of values represented by each axis in the spatial domain
   nbodies: number of bodies (fixed for duration of animation)
   radii: radius of each body, in pixels
   ncolors: the number of different colors that will be used for bodies
   colors: the ncolors colors
   bcolors: the color of each body, specified as an index into colors
   filename: name of file to create, where data will be written
*/
ANIM_File ANIM_Create_nbody(int dim, int lengths[dim],
			    ANIM_range_t ranges[dim],
			    int nbodies, int radii[nbodies],
			    int ncolors,  ANIM_color_t colors[ncolors],
			    int bcolors[nbodies], char * filename);


/* Computes the size of an ANIM file, in bytes, with kind ANIM_NBODY,
   nbodies bodies, and nframes frames */
size_t ANIM_Nbody_file_size(int dim, int nbodies, int ncolors, size_t nframes);


#endif
