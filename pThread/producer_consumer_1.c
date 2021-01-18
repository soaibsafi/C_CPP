#include <pthread.h>
#include <stdio.h>

int buffer;
char data_ready = 0;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *consumer(void *arg)
{
    while (1)
    {
        while (data_ready == 0)
        {
            int temp = buffer;
            data_ready = 0;
            consume_item(temp);
        }
    }
}

void *producer(void *arg)
{
    while (1)
    {
        buffer = produce_item();
        data_ready = 1;
        while (data_ready == 1);
    }
}