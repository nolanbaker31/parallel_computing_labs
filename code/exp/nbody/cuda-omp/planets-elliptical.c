#include "nbody.h"

const double x_min = -2.50e11, x_max = 2.50e11, y_min = .5*-2.50e11, y_max = .5*2.50e11;
const int nx = 1000;
const int nstep = 30000;
const int wstep = 10;
const int ncolors = 5;
const int nbodies = 5;
const double delta_t = 1000;

// https://en.wikipedia.org/wiki/Web_colors#Web-safe_colors
const int colors[][3] = {
  { 255, 255,   0 },   // 0: yellow
  { 112, 128, 144 },   // 1: slate gray
  { 255, 255, 224 },   // 2: light yellow
  { 135, 206, 250 },   // 3: light sky blue
  { 255,  69,   0 },   // 4: orange-red
};

// x, y, vx, vy, mass, color, radius:
const Body bodies[] = {
  { 0.000e00, 0.000e00, 0.000e00, 0.000e00, 1.989e30,   0,   50 },  // sun
  { 5.790e10, 0.000e00, 0.000e00, 2.395e04, 3.302e23,   1,   10 },  // mercury
  { 1.082e11, 0.000e00, 0.000e00, 1.750e04, 4.869e24,   2,   20 },  // venus
  { 1.496e11, 0.000e00, 0.000e00, 1.490e04, 5.974e24,   3,   25 },  // earth
  { 2.279e11, 0.000e00, 0.000e00, 1.205e04, 6.419e23,   4,   15 },  // mars
};  
