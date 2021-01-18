#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<errno.h>

#define NUM_PHIL 10

pthread_mutex_t fork_locks[NUM_PHIL];

int exit_flag = 0;
void *philosopher(void *argc)
{
    int id = *((int *)argc);         //id of the philosopher
    int left = id;                   // id of the left fork
    int right = (id + 1) % NUM_PHIL; // id of the right fork

    while (exit_flag == 0)
    {
        int first = left;
        int second = right;

        /* Lock the first fork */
        pthread_mutex_lock(&fork_locks[first]);
        printf("Philosopher %d picked up the first fork %d\n", id, first);
        fflush(stdout);

        /* Lock the second fork */
        int ret = pthread_mutex_trylock(&fork_locks[second]);
        if (ret == EBUSY)
        {
            printf("Philosopher %d didn't get the second fork %d\n", id, second);
            fflush(stdout);
        }
        else
        {
            printf("Philosopher %d picked up the second fork %d\n", id, second);
            fflush(stdout);

            printf("Philosopher %d is eating.\n", id);
            fflush(stdout);

            // unlock the second fork
            pthread_mutex_unlock(&fork_locks[second]);
            printf("Philosopher %d put back the second fork %d.\n", id, second);
            fflush(stdout);
        }
        // Unlock the first fork
        pthread_mutex_unlock(&fork_locks[first]);
        printf("Philosopher %d put back the first fork %d.\n", id, first);
        fflush(stdout);

        printf("Philosopher %d is thinking.\n", id);
        fflush(stdout);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    int i;

    //initialize the lock for all forks
    for (i = 0; i < NUM_PHIL; i++)
    {
        pthread_mutex_init(&fork_locks[i], NULL);
    }

    // Create thread for each thread
    pthread_t threads[NUM_PHIL];
    int ids[NUM_PHIL];
    for (i = 0; i < NUM_PHIL; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // Wait for key press, and then tell threads to exit
    getchar();
    exit_flag = 1;

    // Wait for all threads
    for (i = 0; i < NUM_PHIL; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the locks for all forks
    for (i = 0; i < NUM_PHIL; i++)
    {
        pthread_mutex_destroy(&fork_locks[i]);
    }

    printf("Finish program");

    return EXIT_SUCCESS;
}