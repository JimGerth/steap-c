#ifndef __QUEUE_H__
#define __QUEUE_H__


/*
 * A queue of generic data.
 *
 * This queue serves as a "first in, first out" buffer, keeping track of generic
 * data. New data can be inserted into the queue (submitted), while data already
 * in the queue can be removed again (retrieved). Data will be retrieved in the
 * same order it was previously submitted.
 */
typedef struct queue_t queue_t;

/*
 * Create a new queue_t object.
 *
 * This will create and initialize a new queue and return a handle to it. That
 * handle can be used with the various queue_*() functions to manipulate the
 * queue, by passing it in as the first argument.
 *
 * This will return NULL if the queue could not be created.
 */
queue_t *
queue_create(void);

/*
 * Destroy a queue_t object.
 *
 * This will destroy a queue_t object, that is not needed anymore. This includes
 * freeing up any memory associated with the queue_t object for example.
 *
 * No more left over data can be retrieved from the queue after destroying it.
 */
void
queue_destroy(queue_t **);

/*
 * Add the given data to the given queue.
 *
 * This call is synchronized and thus safe to call from different threads.
 */
void
queue_submit(queue_t *, void *);

/*
 * Remove and return the oldest data from the given queue.
 *
 * This call will return that data from the queue that has been inserted the
 * longest time ago, operating according to the "first in, first out" principle.
 *
 * This call is synchronized and thus safe to call from different threads.
 *
 * This call will block if the given queue is empty. It will only return once
 * data could successfully be retrieved from the queue.
 */
void *
queue_retrieve(queue_t *);



#ifdef __QUEUE_INTERNAL__

#include <pthread.h>
#include <stdbool.h>


typedef struct queue_item_t {
    struct queue_item_t *next;
    void *data;
} queue_item_t;

struct queue_t {
    queue_item_t *head;
    queue_item_t *tail;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
};


/*
 * Create a new queue_item_t object.
 *
 * This will create and initialize a new queue item and return a handle to it.
 * That handle can be used with the various queue_item_*() functions to
 * manipulate the queue item, by passing it in as the first argument.
 *
 * This will return NULL if the queue_item_t could not be created.
 */
queue_item_t *
queue_item_create(void *);

/*
 * Initialize a new queue item.
 *
 * This must be called for any newly declared queue_item_t, before calling any
 * other queue_* function with it.
 */
void
queue_item_init(queue_item_t *, void *);

/*
 * Destroy a queue_item_t object.
 *
 * This will destroy a queue_item_t object, that is not needed anymore. This
 * includes freeing up any memory associated with the queue_item_t object for
 * example.
 */
void
queue_item_destroy(queue_item_t **);

/*
 * Initialize a new queue.
 *
 * This must be called for any newly declared queue_t instance, before calling
 * any other queue_* function with it.
 */
void
queue_init(queue_t *);

/*
 * Return whether the given queue is empty.
 */
bool
queue_is_empty(queue_t *);

#endif

#endif
