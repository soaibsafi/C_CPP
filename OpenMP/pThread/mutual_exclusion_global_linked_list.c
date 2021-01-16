#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

typedef struct _node_t{
    int index; 
    int *data;
    struct node *next;
} node_t;

typedef struct _list_t{
    node_t *first;
    pthread_mutex_t mutex;
} list_t;

// Initialization of the data structure
int list_init(list_t *listp){
    listp->first = NULL;
    listp->mutex = PTHREAD_MUTEX_INITIALIZER;
    return 1;
}

// Insert into the list elements
void list_insert(int newindex, void *newdata, list_t *listp){
    node_t *curr, *prev, *new;
    int found = 0;
    pthread_mutex_lock(&listp->mutex);

    for(curr = listp->first; curr !=NULL; curr = curr->next){
        prev = curr;
        if(curr->index == newindex){
            found = 1;
            break;
        }
        if(curr->index > newindex){
            break;
        }
    }
    if(!found){
        new = (node_t*)malloc(sizeof(node_t));
        new->index = newindex;
        new->data = newdata;
        new->next = curr->next;
        if(curr == listp->first) listp->first = new;
        else prev->next = new;
    }
    pthread_mutex_unlock(&listp->mutex);
}