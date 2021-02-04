/*******************************
*** TASK-1
*** NAME: - SOAIBUZZAMAN
*** Matrikel Number: 613488
*********************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define NUM_WORKER 10

pthread_mutex_t l_glove[NUM_WORKER];
pthread_mutex_t l_wrench; // used to lock the wrench

int exit_flag = 0;
void *worker(void *id)
{
    int sts = *((int *)id);
    int left_glove = sts;
    int right_glove = (sts + 1) % NUM_WORKER;

    while (exit_flag == 0)
    {
        int first = left_glove;
        int second = right_glove;

        // Lock the left glove
        pthread_mutex_lock(&l_glove[first]);
        printf("Worker %d picked up the left glove %d\n", sts, first);
        fflush(stdout);

        // Try to lock the second glove
        int ret_glove = pthread_mutex_trylock(&l_glove[second]);
        if (ret_glove == EBUSY)
        {
            printf("Worker %d didn't get the right glove %d\n", sts, second);
            fflush(stdout);
        }
        else
        {
            printf("Worker %d picked up the right glove %d\n", sts, second);
            fflush(stdout);

            // Try to lock the wrench after getting the right glove
            int wrench_ret = pthread_mutex_trylock(&l_wrench);
            if (wrench_ret == EBUSY)
            {
                printf("Worker %d didn't get the wrench.\n", sts);
                fflush(stdout);
            }
            else
            {
                printf("Worker %d picked up the wrench.\n", sts);
                fflush(stdout);

                printf("Worker %d tighten the bolts on the casing.\n", sts);
                fflush(stdout);

                // Unlock the wrench
                pthread_mutex_unlock(&l_wrench);
                printf("Worker %d put back the wrench.\n", sts);
                fflush(stdout);
            }
            // Unlock the right glove
            pthread_mutex_unlock(&l_glove[second]);
            printf("Worker %d put back the right glove %d.\n", sts, second);
            fflush(stdout);
        }
        // Unlock the left glove
        pthread_mutex_unlock(&l_glove[first]);
        printf("Worker %d put back the left glove %d.\n", sts, first);
        fflush(stdout);

        printf("Worker %d is connecting the parts.\n", sts);
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    int i;

    //initialize the mutex for all gloves
    for (i = 0; i < NUM_WORKER; i++)
    {
        pthread_mutex_init(&l_glove[i], NULL);
    }

    // initialize the mutex for wrench
    pthread_mutex_init(&l_wrench, NULL);

    // Create thread for each worker
    pthread_t threads[NUM_WORKER];
    int ids[NUM_WORKER];
    for (i = 0; i < NUM_WORKER; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }

    // Wait for key press, and then tell threads to exit
    getchar();
    exit_flag = 1;

    // Wait for all threads
    for (i = 0; i < NUM_WORKER; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the locks for all forks
    for (i = 0; i < NUM_WORKER; i++)
    {
        pthread_mutex_destroy(&l_glove[i]);
    }
    pthread_mutex_destroy(&l_wrench);

    printf("Finish program");

    return EXIT_SUCCESS;
}