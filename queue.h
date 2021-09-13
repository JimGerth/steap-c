#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>
#include <pthread.h>


/*
 * How many items any queue_t instance can hold at a time.
 */
#define QUEUE_LENGTH 5

/*
 * A queue of generic items.
 *
 * This queue serves as a "first in, first out" buffer, keeping track of
 * references to generic data (items). New items can be inserted into the queue
 * (submitted), while items in the queue can be removed again (retrieved). Items
 * will be retrieved in the same order they were previously submitted.
 */
typedef struct queue_t {
    void *items[QUEUE_LENGTH];
    int head;
    int tail;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} queue_t;

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
queue_empty(queue_t *);

/*
 * Return whether the given queue is full.
 */
bool
queue_full(queue_t *);

/*
 * Add the given item to the given queue.
 *
 * This call is synchronized and thus safe to call from different threads.
 *
 * This call will block if the given queue is full. It will only return once the
 * given item could successfully be added to the queue.
 */
void
queue_submit(queue_t *, void *);

/*
 * Remove and return the oldest item from the given queue.
 *
 * This call will return that item from the queue that has been inserted the
 * longest time ago, operating according to the "first in, first out" principle.
 *
 * This call is synchronized and thus safe to call from different threads.
 *
 * This call will block if the given queue is empty. It will only return once an
 * item could successfully be retrieved from the queue.
 */
void *
queue_retrieve(queue_t *);

#endif /* !_QUEUE_H_ */