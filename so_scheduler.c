#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util/so_scheduler.h"
#include "queue.h"

typedef struct {
    tid_t tid;
    unsigned int priority;
    so_handler *handler;
    unsigned int quantum;
    sem_t semaphore;
    char state;
    // 0 = new, 1 = ready, 2 = running, 3 = waiting, 4 = terminated
} thread;

typedef struct {
    bool init;
    unsigned int time_quantum;
    unsigned int io;
    thread* current;
    queue* ready_t;
    queue* finished_t;
    queue* running_t;
    unsigned int number_of_threads;
} scheduler;

//Global variables

static scheduler sched;

int so_init(unsigned int time_quantum, unsigned int io)
{
    if(sched.init || time_quantum == 0
                  || io > SO_MAX_NUM_EVENTS)
       return -1;
    
    sched.init = true;
    sched.time_quantum = time_quantum;
    sched.io = io;
    sched.ready_t = create_queue();
    if(!sched.ready_t)
        return -2;

    sched.running_t = create_queue();
    if(!sched.running_t) {
        free(sched.ready_t);
        return -2;
    }

    sched.finished_t = create_queue();
    if(!sched.finished_t) {
        free(sched.ready_t);
        free(sched.running_t);
        return -2;
    }

    return 0;
}

void* start_routine(void* arg)
{
    thread* t = (thread*) arg;

    if(sched.current == NULL) {
        t->state = 2;
        sched.current = t;
    }
    else {
        sem_wait(&t->semaphore);
    }
    t->handler(t->priority);
    t->state = 4;
    enqueue(sched.finished_t, t);
    
    sem_destroy(&t->semaphore);

    return (void*)t->tid;
}

void init_thread(thread* t, so_handler* func, unsigned int priority)
{
    t->priority = priority;
    t->handler = func;
    t->quantum = sched.time_quantum;
    sem_init(&t->semaphore, 0, 0);
    t->state = 0;
    ++sched.number_of_threads;
}

tid_t so_fork(so_handler* func, unsigned int priority)
{
    if(priority > SO_MAX_PRIO || func == NULL)
        return INVALID_TID;

    thread* t = malloc(sizeof(thread));
    if(!t)
        exit(EXIT_FAILURE);

    init_thread(t, func, priority);

    pthread_create(&t->tid, NULL, start_routine, t);

    sem_post(&t->semaphore);

    void* retval;
    pthread_join(t->tid, &retval);
    free(t);

    return (tid_t)retval;
}

int so_wait(unsigned int io)
{   
    if (io >= sched.io)
		return -1;

    return 0;
}

int so_signal(unsigned int io)
{   
    if (io >= sched.io)
		return -1;

    return 0;
}

void so_exec(void)
{
    if(sched.current == NULL)
        return;
}

void so_end(void)
{   
    if(!sched.init)
        return;

    sched.init = false;
    if(sched.running_t != NULL)
        free_queue(&sched.running_t);
    if(sched.ready_t != NULL)
        free_queue(&sched.ready_t);
    if(sched.finished_t != NULL)
        free_queue(&sched.finished_t);
}