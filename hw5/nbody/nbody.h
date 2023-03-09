#ifndef NBODY
#define NBODY

/* The current state of a body: position and velocity */
typedef struct StateStruct {
  double x;        /* x position */
  double y;        /* y position */
  double vx;       /* velocity, x-direction */
  double vy;       /* velocity, y-direction */
} State;

/* There is one structure of this type for each "body" in the
   simulation.  All of the attributes and the current state of the
   body are recorded in this structure. */
typedef struct BodyStruct {
  State state;     /* the current state of this body */
  double mass;     /* mass of body */
  int color;       /* color used to draw this body */
  int radius;      /* radius of body in pixels */
} Body;

#endif
