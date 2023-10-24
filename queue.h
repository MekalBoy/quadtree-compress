/* TACHE Robert-Andrei - 312CD */

#pragma once

#include "tree.h"

typedef struct list {
    Tree value;
    struct list *next;
} *List;

typedef struct queue {
    List front, rear;
} Queue;

Queue* initQueue();
int isEmptyQueue(Queue *q);
void enqueue(Queue *q, Tree node);
Tree dequeue(Queue *q);
