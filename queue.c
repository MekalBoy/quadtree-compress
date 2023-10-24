/* TACHE Robert-Andrei - 312CD */

#include "tree.h"
#include "queue.h"

Queue* initQueue() {
    Queue *q = (Queue*) malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

List createList(Tree value) {
    List head = malloc(sizeof(struct list));
    head->value = value;
    head->next = NULL;
    return head;
}

int isEmptyQueue(Queue *q) {
    return q->front == NULL;
}

void enqueue(Queue *q, Tree node) {
    if (q->front == NULL) {
        q->front = q->rear = createList(node);
    } else {
        List list = createList(node);
        q->rear->next = list;
        q->rear = list;
    }
}

Tree dequeue(Queue *q) {
    if (q->front == NULL) return NULL;

    Tree result = q->front->value;
    List tmp = q->front;
    q->front = q->front->next;

    if (tmp->next == NULL) {
        q->rear = NULL;
    }

    free(tmp);
    return result;
}
