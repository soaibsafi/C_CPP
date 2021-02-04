/*******************************
*** *** TASK-2
*** NAME: - SOAIBUZZAMAN
*** Matrikel Number: 613488
*********************************/

#include <pthread.h>
#include <stdio.h>

pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

typedef struct _s_val
{

} sval;
sval start_value;
int start_value_set = 0;

// Run on single thread
void *init_function(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&m);
        sval temp = inital_computation();
        start_value = temp;
        start_value_set = 1;
        pthread_cond_broadcast(&c); // broadcast-> because executed by n>1 threads.
        pthread_mutex_unlock(&m);
    }
}

// Run on n>1 threads.
void *parallel_exec(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&m);
        while (start_value_set == 0)
        {
            pthread_cond_wait(&c, &m); //unlock(m); wait(c); lock(m);
        }
        sval temp = start_value;
        parallel_computation(temp);
        pthread_mutex_unlock(&m);
    }
}