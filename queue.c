#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define __QUEUE_INTERNAL__
#include "queue.h"


queue_item_t *
queue_item_create(void *data) {
    queue_item_t *item;
    if ((item = malloc(sizeof (queue_item_t))) == NULL) {
        return NULL;
    }

    queue_item_init(item, data);
    return item;
}

void
queue_item_destroy(queue_item_t **item) {
    if (item == NULL) {
        return;
    }

    free(*item);
    *item = NULL;
}

void
queue_item_init(queue_item_t *item, void *data) {
    if (item == NULL) {
        return;
    }

    item->next = NULL;
    item->data = data;
}

queue_t *
queue_create(void) {
    queue_t *queue;
    if ((queue = malloc(sizeof (queue_t))) == NULL) {
        return NULL;
    }

    queue_init(queue);
    return queue;
}

void
queue_destroy(queue_t **queue) {
    if (queue == NULL) {
        return;
    }

    while (!queue_is_empty(*queue)) {
        queue_item_t *item = queue_retrieve(*queue);
        queue_item_destroy(&item);
    }

    free(*queue);
    *queue = NULL;
}

void
queue_init(queue_t *queue) {
    if (queue == NULL) {
        return;
    }

    queue->head = NULL;
    queue->tail = NULL;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
}

bool
queue_is_empty(queue_t *queue) {
    if (queue == NULL) {
        return true;
    }

    return queue->head == NULL;
}

void
queue_submit(queue_t *queue, void *data) {
    if (queue == NULL) {
        return;
    }

    queue_item_t *item;
    while ((item = queue_item_create(data)) == NULL);

    pthread_mutex_lock(&queue->mutex);

    if (queue_is_empty(queue)) {
        queue->head = item;
    } else {
        queue->tail->next = item;
    }

    queue->tail = item;

    pthread_cond_signal(&queue->not_empty);

    pthread_mutex_unlock(&queue->mutex);
}

void *
queue_retrieve(queue_t *queue) {
    if (queue == NULL) {
        return NULL;
    }

    pthread_mutex_lock(&queue->mutex);

    while (queue_is_empty(queue)) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }

    queue_item_t *item = queue->head;

    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    pthread_mutex_unlock(&queue->mutex);

    void *data = item->data;

    queue_item_destroy(&item);

    return data;
}
