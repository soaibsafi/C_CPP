#include<stdio.h>
#include<pthread.h>
#include <stdlib.h>
#include<errno.h>

#define NUM_THREADS 5

void* PrintHello(void* threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello from thread %ld\n", tid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for(t=0; t<NUM_THREADS; t++)
    {
        rc = pthread_create(&threads[t],NULL, PrintHello, (void*)t);
        if(rc){
            printf("Error: return code from pthread_create() %d\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}