#include <stdio.h>
#include <pthread.h>

typedef struct _thrarg_t
{
    int rows, cols;
} thrarg_t;

void *loopthread(void *arg)
{
    int j, k;
    thrarg_t *thrarg = (thrarg_t *)arg;
    for (j = 0; j < thrarg->rows; j++)
        for (k = 0; k < thrarg->cols; k++)
            printf("Loop Iteration %d\t%d\n", j, k);
    return NULL;
}

int main(int argc, char **argv)
{
    // pthread_t tid;
    // thrarg_t thrarg;
    // thrarg.rows = 10;
    // thrarg.cols = 10;

    // pthread_create(&tid, NULL, loopthread, &thrarg);
    // pthread_join(tid, NULL);

    // Race condition
    pthread_t tid1, tid2;
    thrarg_t thrarg;
    thrarg.rows = 5;
    thrarg.cols = 10;
    pthread_create(&tid1, NULL, loopthread, &thrarg);
    thrarg.rows = 10;
    thrarg.cols = 20;
    pthread_create(&tid2, NULL, loopthread, &thrarg);

    //Waiting for the termination of different thread
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}