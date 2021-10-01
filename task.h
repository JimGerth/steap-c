/*
 * The task module.
 */
#ifndef __TASK_H__
#define __TASK_H__


/*
 * An arbitrary action to be executed as part of a task.
 *
 * A task_action_t function accepts a generic void* argument which it can use in
 * its execution.
 */
typedef void (*task_action_t)(void *);

/*
 * An abstract encapsulation of some computative work to be done.
 *
 * A task can be thought of as an independent unit of work. This could be a
 * mathmatical computation, some file input/output, or anything else.
 *
 * The actual code to be executed for this task is in the action function. The
 * argument will be passed into the action function when it is called.
 */
typedef struct task_t task_t;

/*
 * Create a new task_t object.
 *
 * This will create and initialize a new task and return a handle to it. That
 * handle can be used with the various task_*() functions to manipulate the
 * task, by passing it in as the first argument.
 *
 * This will return NULL if the task could not be created.
 */
task_t *
task_create(task_action_t, void *);

/*
 * Destroy a task_t object.
 *
 * This will destroy a task_t object, that is not needed anymore. This includes
 * freeing up any memory associated with the task_t object for example.
 */
void
task_destroy(task_t **);

/*
 * Execute the given task.
 *
 * This will hand execution over to the task's action function, passing in the
 * task's supplied argument.
 */
void
task_execute(task_t *);


/*
 * The following is for internal use only.
 */
#ifdef __TASK_INTERNAL__


struct task_t {
    task_action_t action;
    void *argument;
};

/*
 * Initialize a new task.
 *
 * This must be provided with a task action function and an argument for that
 * function.
 */
void
task_init(task_t *, task_action_t, void *);


#endif /* __TASK_INTERNAL__ */

#endif /* __TASH_H__ */
