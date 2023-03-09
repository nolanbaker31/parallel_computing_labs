
/* Interface for a multithreaded barrier */

/* Global barrier state type, an opaque handle.  One barrier state
   object is created and shared by all threads. */
typedef struct barrier_state * barrier_state_t;

/* The barrier type, an opaque handle.  Each thread creates its own
   barrier object, which possibly contains a reference to the single
   global barrier state object. */
typedef struct barrier * barrier_t;

/* Creates a new barrier state object for given number of threads,
   returning a handle to it.  Should be called once by a single
   thread, typically by the main thread before the team of threads
   that will use the barrier is created. */
barrier_state_t barrier_state_create(int nthreads);

/* Destroys the barrier state object.  Should be called once, by a
   single thread, typically the same thread that called
   barrier_state_create().  It should be called only after each thread
   in the team has called barrier_destroy(). */
void barrier_state_destroy(barrier_state_t bs);

/* Creates a (local) barrier object associated to the given barrier
   state object.  Each thread in the team should call this function
   and save the handle returned, which will be used in subsequent
   calls to barrier_wait().

   The client also specifies the ID number.  The ID numbers of
   0,1,...,nthreads-1.  If two threads have the same ID numbers, the
   ID number is out of this range, or some thread does not call
   barrier_create, the behavior is undefined.
*/
barrier_t barrier_create(barrier_state_t bs, int id);

/* Destroys the (local) barrier object.  The barrier cannot be used
   after this function is called.  Each thread should call this on its
   own handle. */
void barrier_destroy(barrier_t bar);

/* Invokes the barrier --- no thread can return until all threads have
   called this function. */
void barrier_wait(barrier_t bar);
