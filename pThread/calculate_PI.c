#include <stdio.h>
#include <pthread.h>

typedef struct _param
{
    int lower_bound;
    int upper_bound;
    double result;
    double delta;
} param;

void *thread_func(void *arg)
{
    int j;
    double x;
    param *par = (param *)arg;
    par->result = 0.0;
    for (j = par->lower_bound; j <= par->upper_bound; j++)
    {
        x = ((double)j - 0.5) * par->delta;
        par->result += 4.0 / (1.0 + (x * x));
    }
    pthread_exit(NULL);
}

#define NUM_THREADS 24

int main(int argc, char **argv)
{
    pthread_t tid[NUM_THREADS];
    param par[NUM_THREADS];

    int intervals = 100000000;
    double pi = 0.0;

    int blocksize = (intervals+NUM_THREADS-1)/NUM_THREADS;  //ceil()
    // Create thread
    for (int i = 0; i < NUM_THREADS; i++)
    {

        par[i].lower_bound = i*blocksize;
        par[i].upper_bound = (i+1)*blocksize-1;
        if(par[i].upper_bound > intervals) par[i].upper_bound = intervals;
        par[i].delta = 1.0 / (double)intervals;



        pthread_create(&tid[i], NULL, thread_func, &par[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
        pi += par[i].result;
    }
    pi /= intervals;

    printf("The value of pi is %f\n", pi);

    return 0;
}