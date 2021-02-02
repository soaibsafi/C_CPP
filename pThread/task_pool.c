#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// Data structure for a single task
typedef struct _work_t
{
    void (*routine)(void *); // Function to be executed
    void *arg;
    struct _work_t *next;
} work_t;

// Data structure fo the whole tasl pool
typedef struct _tpool_t
{
    int num_threads;          //Number of thread
    int max_size, curr_size;  // max/current number of task in the pool
    pthread_t *threads;       // Field with threads
    work_t *head, *tail;      // Head, tail of the queue
    pthread_mutex_t lock;     // Access control
    pthread_cond_t not_empty; // Tasks are available
    pthread_cond_t not_full;  // Tasks can be executed
} tpool_t;

// Work function
void *tpool_thread(void *vtpl)
{
    tpool_t *tpl = (tpool_t *)vtpl;
    work_t *wl;

    while (1)
    {
        pthread_mutex_lock(&tpl->lock);
        while (tpl->curr_size == 0)
            pthread_cond_wait(&tpl->not_empty, &tpl->lock);
        wl = tpl->head;
        tpl->curr_size--;

        if (tpl->curr_size == 0)
            tpl->head = tpl->tail = NULL;
        else
            tpl->head = wl->next;

        if (tpl->curr_size == tpl->max_size - 1) //Pool full
            pthread_cond_signal(&tpl->not_full);
        pthread_mutex_unlock(&tpl->lock);
        (*(wl->routine))(wl->arg);
        free(wl);
    }
}

//Initialization
tpool_t *tpool_init(int num_threads, int max_size)
{
    tpool_t *tpl = (tpool_t *)malloc(sizeof(tpool_t));
    tpl->num_threads = num_threads;
    tpl->max_size = max_size;
    tpl->curr_size = 0;
    tpl->head = tpl->tail = NULL;

    pthread_mutex_init(&tpl->lock, NULL);
    pthread_cond_init(&tpl->not_empty, NULL);
    pthread_cond_init(&tpl->not_full, NULL);

    tpl->threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(tpl->threads + i, NULL, tpool_thread, (void *)tpl);
    }

    return tpl;
}

// Adding task
void tpool_insert(tpool_t *tpl, void (*f)(void *), void *arg)
{
    work_t *wl;
    pthread_mutex_lock(&tpl->lock);
    while (tpl->curr_size == tpl->max_size)
    {
        pthread_cond_wait(&tpl->not_full, &tpl->lock);
    }
    wl = (work_t *)malloc(sizeof(work_t));
    wl->routine = f;
    wl->arg = arg;
    wl->next = NULL;

    if (tpl->curr_size == 0)
    {
        tpl->head = tpl->tail = wl;
        pthread_cond_signal(&tpl->not_empty);
    }
    else
    {
        tpl->tail->next = wl;
        tpl->tail = wl;
    }

    tpl->curr_size++;
    pthread_mutex_unlock(&tpl->lock);
}