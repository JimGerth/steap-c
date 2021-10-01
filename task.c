#include <stdlib.h>

#define __TASK_INTERNAL__
#include "task.h"


task_t *
task_create(task_action_t action, void *argument) {
    task_t *task;
    if ((task = malloc(sizeof (task_t))) == NULL) {
        return NULL;
    }

    task_init(task, action, argument);
    return task;
}

void
task_destroy(task_t **task) {
    if (task == NULL) {
        return;
    }

    free(*task);
    *task = NULL;
}

void
task_init(task_t *task, task_action_t action, void *argument) {
    if (task == NULL) {
        return;
    }

    task->action = action;
    task->argument = argument;
}

void
task_execute(task_t *task) {
    if (task == NULL) {
        return;
    }

    task->action(task->argument);
}
