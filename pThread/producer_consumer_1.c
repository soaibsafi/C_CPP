#include <pthread.h>
#include <stdio.h>

int buffer;
char data_ready = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *consumer(void *arg)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        while (data_ready == 0)
        {
            // wait for elements in buffer
            pthread_cond_wait(&cond, &mutex);  //wait for signal to cond
            int temp = buffer;
            data_ready = 0;
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
            consume_item(temp);
        }
    }
}

void *producer(void *arg)
{
    while (1)
    {
        int temp = produce_item();
        pthread_mutex_lock(&mutex);
        buffer = temp;
        data_ready = 1;
        pthread_cond_signal(&cond);
        while (data_ready == 1){
            // waiting for the buffer to be empty
            pthread_cond_wait(&cond, &mutex);  // do 3 steps- unlock(mutex); wait(cond); lock(mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
}