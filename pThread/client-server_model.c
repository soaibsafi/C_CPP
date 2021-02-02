#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define REQ_READ 1
#define REQ_WRITE 2
#define REQ_QUIT 3
#define PROMPT_SIZE 32
#define TEXT_SIZE 128

typedef struct _request_t
{
    struct _request_t *next; // Linked list
    int op;
    int synchronous; // 1 if clients waits for server
    int done_flag;
    pthread_cond_t done;
    char prompt[PROMPT_SIZE], text[TEXT_SIZE];
} request_t;

typedef struct _tty_server_t
{
    request_t *first, *last;
    int running;
    pthread_mutex_t m;
    pthread_cond_t request;
} tty_server_t;

#define TTY_SERVER_INITIALIZER                                             \
    {                                                                      \
        NULL, NULL, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER \
    }

tty_server_t tty_server = TTY_SERVER_INITIALIZER;

int client_threads;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t client_done = PTHREAD_COND_INITIALIZER;
pthread_t server_thread;

// Server thread
void *tty_server_routine(void *arg)
{
    static pthread_mutex_t prompt_mutex = PTHREAD_MUTEX_INITIALIZER;

    request_t *request;

    while (1)
    {
        pthread_mutex_lock(&tty_server.m);
        while (tty_server.first == NULL)
        {
            pthread_cond_wait(&tty_server.request, &tty_server.m);
        }
        request = tty_server.first;
        tty_server.first = request->next;

        if (tty_server.first == NULL)
            tty_server.last = NULL;
        pthread_mutex_unlock(&tty_server.m);
        int op = request->op;
        switch (op)
        {
        case REQ_READ:
            puts(request->prompt);
            if (fgets(request->text, TEXT_SIZE, stdin) == NULL)
                request->text[0] = '\0';
            int len = strlen(request->text);
            if (len > 0 && request->text[len - 1] == '\n')
                request->text[len - 1] = '\0';
            break;
        case REQ_WRITE:
            puts(request->text);
            break;
        default:
            break;
        }
        if (request->synchronous)
        {
            pthread_mutex_lock(&tty_server.m);
            request->done_flag = 1;
            pthread_cond_signal(&request->done);
            pthread_mutex_unlock(&tty_server.m);
        }
        else
        {
            free(request);
        }
        if (op == REQ_QUIT)
            break;
    }
    return NULL;
}

// Passing a request to the server
void tty_server_request(int op, int sync, char *prompt, char *string)
{
    request_t *request;
    pthread_mutex_lock(&tty_server.m);
    {
        if (!tty_server.running)
        {
            pthread_create(&server_thread, NULL, tty_server_routine, NULL);
            tty_server.running = 1;
        }
        request = (request_t *)malloc(sizeof(request_t));
        request->op = op;
        request->synchronous = sync;
        request->next = NULL;

        if (sync)
        {
            request->done_flag = 0;
            pthread_cond_init(&request->done, NULL);
        }
        if (prompt != NULL)
        {
            strcpy(request->prompt, prompt, PROMPT_SIZE);
            request->prompt[PROMPT_SIZE - 1] = '\0';
        }
        else
        {
            request->prompt[0] = '\0';
        }
        if (op == REQ_WRITE && string != NULL)
        {
            strcpy(request->text, string, TEXT_SIZE);
            request->text[TEXT_SIZE - 1] = '\0';
        }
        else
            request->text[0] = '\0';

        if (tty_server.first == NULL)
            tty_server.first = tty_server.last = request;
        else
        {
            tty_server.last->next = request;
            tty_server.last = request;
        }

        pthread_cond_signal(&tty_server.request);
        if (sync)
        {
            while (!request->done_flag)
                pthread_cond_wait(&request->done, &tty_server.m);
            if (op == REQ_READ)
                strcpy(string, request->text);
            pthread_cond_destroy(&request->done);
            free(request);
        }
    }
    pthread_mutex_unlock(&tty_server.m);
}

// Client structure