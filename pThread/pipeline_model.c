#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct _stage_t
{
  pthread_mutex_t m;
  pthread_cond_t avail;
  pthread_cond_t ready;
  int data_ready;
  long data;
  pthread_t thread;
  struct _stage_t *next;
} stage_t;

typedef struct _pipe_t
{
  pthread_mutex_t m;
  stage_t *head, *tail;
  int stages;
  int active;
} pipe_t;

// Function for passing data to a pipeline stage
int pipe_send(stage_t *stage, long data)
{
  pthread_mutex_lock(&stage->m);
  {
    while (stage->data_ready)
      pthread_cond_wait(&stage->ready, &stage->m);
    stage->data = data;
    stage->data_ready = 1;
    pthread_cond_signal(&stage->avail);
  }
  pthread_mutex_unlock(&stage->m);
}

// The start function for the pipeline threads takes a pointer to the corresponding stage as paramente
void *pipe_stage(void *arg)
{
  stage_t *stage = (stage_t *)arg;
  pthread_mutex_lock(&stage->m);
  {
    while (1)
    {
      while (!stage->data_ready) //Wait for data
        pthread_cond_wait(&stage->avail, &stage->m);

      // Process data and pass it to successor stage
      pipe_send(stage->next, (stage->data + 1));
      stage->data_ready = 0; // Process complete
      pthread_cond_signal(&stage->ready);
    }
  }
  pthread_mutex_unlock(&stage->m); // Never reached.
}

// Creation of the pipeline
int pipe_create(pipe_t *pipe, int stages)
{
  stage_t **link = &pipe->head;
  pthread_mutex_init(&pipe->head, NULL);
  pipe->stages = stages;
  pipe->active = 0;
  stage_t *new_stage = (stage_t *)malloc(sizeof(stage_t));

  //Create state + 1 states, last stage for the result
  for (int pi = 0; pi <= stages; pi++)
  {

    pthread_mutex_init(&new_stage->m, NULL);
    pthread_cond_init(&new_stage->avail, NULL);
    pthread_cond_init(&new_stage->ready, NULL);
    new_stage->data_ready = 0;
    *link = new_stage; // Set up of a linkled list
    link = &new_stage->next;
  }
  *link = (stage_t *)NULL;
  pipe->tail = new_stage;

  //Create a thread for each stage except the last one
  for (stage_t *st = pipe->head; st->next; st = st->next)
  {
    pthread_create(&st->thread, NULL, pipe_stage, st);
  }
}

// Passing the value of the pipeline
// This function does not wait for the result and returns directly to the caller instead
// Precessing of the pipeline is asynchronous.
int pipe_start(pipe_t *pipe, long v)
{
  pthread_mutex_lock(&pipe->m);
  {
    pipe->active++;
  }
  pthread_mutex_unlock(&pipe->m);
  pipe_send(pipe->head, v);
}

//Removing value from the pipeline
// The executing thread waits until the value is abailable at the last stage
int pipe_result(pipe_t *pipe, long *result)
{
  stage_t *tail = pipe->tail;
  int empty = 0;
  pthread_mutex_lock(&pipe->m);
  if (pipe->active <= 0)
    empty = 1;
  else
    pipe->active--;

  pthread_mutex_unlock(&pipe->m);

  if (empty)
    return 0;

  pthread_mutex_lock(&tail->m);
  while (!tail->data_ready)
    pthread_cond_wait(&tail->avail, &tail->m);

  *result = tail->data;
  tail->data_ready = 0;

  pthread_cond_signal(&tail->rady);
  pthread_mutex_unlock(&tail->m);
  return 1;
}

// Main program controlling the pipeline processing
int main(int argc, char *argv[])
{
  pipe_t pipe;
  long value, result;
  char line[128];

  pipe_create(&pipe, 10);

  //Program termination only in case of a parsing error or EOF
  while (fgets(line, sizeof(line), stdin))
  {
    if (!line)
      continue;
    if (!strcmp(line, "-"))
    {
      if (pipe_result(&pipe, &result))
      {
        printf("%ld\n", result);
      }
      else
      {
        printf("Error: Pipe Empty");
      }
    }
    else
    {
      if (sscanf(line, "%ld", &value) < 1)
        printf("Error: Not an int");
      else
        pipe_start(&pipe, value);
    }
  }
  return 0;
}