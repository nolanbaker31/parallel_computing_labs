/*  Based on fractal code by Martin Burtscher. */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <cuda.h>
extern "C" {
#include "anim.h"
}
const double Delta = 0.001;
const double xMid =  0.2370104;
const double yMid =  0.5210007;
int width;

static void quit() {
  printf("Usage: mandelbrot.exec WIDTH NSTEP FILENAME            \n\
  WIDTH = frame width, in pixels, (at least 10)                  \n\
  NSTEP = number of frames in the animation (at least 1)         \n\
  FILENAME = name of output file (to be created)                 \n\
Example: mandelbrot.exec 200 100 out.anim                        \n");
  exit(1);
}

__global__ void kernel(double * buf, double * xMin, double * yMin, double * dw){
   // Each block of size width.y gets width.x threads
   const int ltid = threadIdx.x; // local Thread ID
   const int btid = blockIdx.x; // block ID

   int i = ltid;
   const double cx = *xMin + i * *dw;
   int j = btid;
     const double cy = *yMin + j * *dw;
     double x = cx, y = cy, x2, y2;
     int depth = 256;

     do {
	x2 = x * x;
	y2 = y * y;
	y = 2 * x * y + cy;
	x = x2 - y2 + cx;
	depth--;
  } while (depth > 0 && x2 + y2 < 5.1);
  buf[i * blockDim.x + j] = (double)depth;
  __syncthreads();
}


int main(int argc, char *argv[]) {
  if (argc != 4) quit();
  // Setup Variables
  double start_time = ANIM_time();
  int dots = 0, nstep = atoi(argv[2]);
  width = atoi(argv[1]);
  char * filename = argv[3];
  int nblocks = width;   

  if (nstep < 1) quit();
  printf("mandelbrot: creating ANIM file %s with %d frames, %dx%d pixels, %zu bytes.\n",
	 filename, nstep, width, width,
	 ANIM_Heat_file_size_2d(width, width, nstep));

  ANIM_File af =
    ANIM_Create_heat_2d(width, width, 0, width, 0, width, 0, 255, filename);
  double * buf = (double*)malloc(width * width * sizeof(double)), delta = Delta;
  assert(buf);
  // Set Cuda Variables
  int err;
  double *dev_xMin;
  double *dev_yMin;
  double *dev_dw;
  int threadsPerBlock = width;
  double * dev_buf;
  err = cudaMalloc((void**)&dev_buf, width*width*sizeof(double)); assert(err == cudaSuccess);
err = cudaMemcpy(dev_buf, buf, width*width*sizeof(double), cudaMemcpyHostToDevice); assert(err == cudaSuccess);
  err = cudaMalloc((void**)&dev_xMin, sizeof(double)); assert(err == cudaSuccess); assert(err == cudaSuccess);
  err = cudaMalloc((void**)&dev_yMin, sizeof(double)); assert(err == cudaSuccess); assert(err == cudaSuccess);
  err = cudaMalloc((void**)&dev_dw, sizeof(double)); assert(err == cudaSuccess); assert(err == cudaSuccess);
// Begin work
  for (int frame = 0; frame < nstep; frame++) {
    const double xMin = xMid - delta, yMin = yMid - delta;
    const double dw = 2.0 * delta / width;
    // send updated variables to kernel each iteration
    err = cudaMemcpy(dev_xMin, &xMin, sizeof(double), cudaMemcpyHostToDevice);
    err = cudaMemcpy(dev_yMin, &yMin, sizeof(double), cudaMemcpyHostToDevice);
    err = cudaMemcpy(dev_dw, &dw, sizeof(double), cudaMemcpyHostToDevice);
    kernel<<<nblocks, threadsPerBlock>>>(dev_buf, dev_xMin, dev_yMin, dev_dw);
    // update host buffer
    err = cudaMemcpy(buf, dev_buf, width*width*sizeof(double), cudaMemcpyDeviceToHost);
    assert(err == cudaSuccess);
    // update frame
    ANIM_Write_frame(af, buf);
    ANIM_Status_update(stdout, nstep, frame+1, &dots);
    delta *= 0.99;
  }
  ANIM_Close(af);
  printf("\nmandelbrot: finished.  Time = %lf\n", ANIM_time() - start_time);
  free(buf);
  cudaFree(dev_buf);
  cudaFree(dev_xMin);
  cudaFree(dev_yMin);
  cudaFree(dev_dw);
}
