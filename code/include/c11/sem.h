#ifndef _SEM
#define _SEM
/* A basic semaphore library, using C11 atomics.
 * Author: Stephen F. Siegel
 * Date: Nov 2020
 */
#include <stdatomic.h>

/* The opaque type semaphore */
typedef volatile atomic_int semaphore;

/* Initializes the semaphore structure with the given value */
void semaphore_init(semaphore *s, int val);

/* Destroys the semaphore structure */
void semaphore_destroy(semaphore *s);

/* Increments s atomically, and returns the result. */
void semaphore_V(semaphore *s);

/* Waits for s to be positive and decrements atomically. */
void semaphore_P(semaphore *s);

#endif
