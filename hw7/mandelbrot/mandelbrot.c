/*  Based on fractal code by Martin Burtscher. */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "anim.h"

const double Delta = 0.001;
const double xMid =  0.2370104;
const double yMid =  0.5210007;

static void quit() {
  printf("Usage: mandelbrot.exec WIDTH NSTEP FILENAME            \n\
  WIDTH = frame width, in pixels, (at least 10)                  \n\
  NSTEP = number of frames in the animation (at least 1)         \n\
  FILENAME = name of output file (to be created)                 \n\
Example: mandelbrot.exec 200 100 out.anim                        \n");
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc != 4) quit();

  double start_time = ANIM_time();
  int dots = 0, width = atoi(argv[1]), nstep = atoi(argv[2]);
  char * filename = argv[3];

  if (nstep < 1) quit();
  printf("mandelbrot: creating ANIM file %s with %d frames, %dx%d pixels, %zu bytes.\n",
	 filename, nstep, width, width,
	 ANIM_Heat_file_size_2d(width, width, nstep));

  ANIM_File af =
    ANIM_Create_heat_2d(width, width, 0, width, 0, width, 0, 255, filename);
  double * buf = malloc(width * width * sizeof(double)), delta = Delta;

  assert(buf);
  for (int frame = 0; frame < nstep; frame++) {
    const double xMin = xMid - delta, yMin = yMid - delta;
    const double dw = 2.0 * delta / width;
    
    for (int i = 0; i < width; i++) {
      const double cx = xMin + i * dw;
      
      for (int j = 0; j < width; j++) {
        const double cy = yMin + j * dw;
        double x = cx, y = cy, x2, y2;
        int depth = 256;

        do {
          x2 = x * x;
          y2 = y * y;
          y = 2 * x * y + cy;
          x = x2 - y2 + cx;
          depth--;
        } while (depth > 0 && x2 + y2 < 5.1);
	buf[i * width + j] = (double)depth;
      }
    }
    ANIM_Write_frame(af, buf);
    ANIM_Status_update(stdout, nstep, frame+1, &dots);
    delta *= 0.99;
  }
  ANIM_Close(af);
  printf("\nmandelbrot: finished.  Time = %lf\n", ANIM_time() - start_time);
  free(buf);
}
