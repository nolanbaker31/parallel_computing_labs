#ifndef _FLAG
#define _FLAG
/* A basic concurrent flag library, using Pthreads mutexes and condition variables.

   A flag has a value, which is either 0 or 1.   It supports two atomic operations:

      - raise: this can only be executed when the value is 0.  It
        makes the value 1.
      - lower: this blocks until the value is 1, then makes the value
        0 and returns.
   
   Author: Stephen F. Siegel
   Date: Oct 2020
*/
#include <pthread.h>

/* The flag type */
typedef struct flag {
  _Bool val; // true=raised, false=lowered
  pthread_mutex_t mutex;
  pthread_cond_t condition_var;
} flag_t;

/* Initializes the flag with the given value.  Must be called before
   the first time the flag is used. */
void flag_init(flag_t * f, _Bool val);

/* Destroys the flag */
void flag_destroy(flag_t * f);

/* Increments f atomically, and returns the result.  Notifies threads
   waiting for a change on f.  An assertion is violated if f is 1 when
   this function is called. */
void flag_raise(flag_t * f);

/* Waits for f to be 1, then sets it to 0, all atomically. */
void flag_lower(flag_t * f);

#endif
