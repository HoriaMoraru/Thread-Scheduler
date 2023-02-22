#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

queue_node* new_node(void* thread)
{
    queue_node* temp = (queue_node*)malloc(sizeof(queue_node));
    if(!temp)
        return NULL;

    temp->key = thread;
    temp->next = NULL;
    return temp;
}

queue* create_queue()
{
    queue* q = (queue*)malloc(sizeof(queue));
    if(!q)
        return NULL;
    
    q->front = q->back = NULL;
    q->size = size;

    return q;
}

void enqueue(queue* q, void* thread)
{
    queue_node* temp = new_node(thread);

    if(q->back == NULL) {
        q->front= q->back = temp;
        return;
    }

    q->back->next = temp;
    q->back = temp;
}

void dequeue(queue* q)
{
    if(q->front == NULL)
        return;
    
    queue_node* temp = q->front;
    
    q->front = q->front->next;

    if(q->front == NULL)
        q->back = NULL;

    free(temp);
}

int size(queue* q)
{
    int queue_size = 0;

    queue_node* temp = q->front;

    while(temp != NULL) {
        ++queue_size;
        temp = temp->next;
    }

    return queue_size;
}

void free_queue(queue** q)
{
    queue_node* temp = (*q)->front;
    queue_node* next = NULL;

    while(temp != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }

    free(*q);
    *q = NULL;
}