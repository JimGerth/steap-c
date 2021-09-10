#include <stdio.h>

#include "task_queue.h"


void
task_queue_init(void) {
    task_queue.head = 0;
    task_queue.tail = 0;
}

void
task_queue_enqueue(task_t task) {
    if ((task_queue.head + 1) % TASK_QUEUE_LENGTH == task_queue.tail) {
        fprintf(stderr, "Could not enqueue task: The task queue is full!\n");
    }

    task_queue.tasks[task_queue.head++] = task;
    task_queue.head %= TASK_QUEUE_LENGTH;
}

task_t
task_queue_dequeue(void) {
    if (task_queue.head == task_queue.tail) {
        fprintf(stderr, "Could not dequeue task: The task queue is empty!\n");
    }

    task_t task = task_queue.tasks[task_queue.tail++];
    task_queue.tail %= TASK_QUEUE_LENGTH;
    return task;
}