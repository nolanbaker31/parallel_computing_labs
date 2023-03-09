#ifndef _FLAG
#define _FLAG
/* Interface for concurrency flags using C11 atomics. */
#include <stdatomic.h>

typedef volatile atomic_flag flag_t;

/* Initializes the flag with the given value.  Must be called before
   the first time the flag is used. */
void flag_init(flag_t * f, _Bool val);

/* Destroys the flag */
void flag_destroy(flag_t * f);

/* Increments f atomically, and returns the result.  Notifies threads
   waiting for a change on f. */
void flag_raise(flag_t * f);

/* Waits for f to be positive, then decrements it, all atomically. */
void flag_lower(flag_t * f);

#endif
