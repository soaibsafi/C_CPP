#include <pthread.h>
#include <stdio.h>

int cond;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void* thread_1(void* arg)
{
    pthread_mutex_lock(&m);
    cond = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

void* thread_2(void* arg)
{
    pthread_mutex_lock(&m);
    while(!cond){    // while instead of if
        pthread_cond_wait(&c, &m);
    }

    pthread_mutex_unlock(&m);
}