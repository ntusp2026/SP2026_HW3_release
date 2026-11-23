#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

void queue_init(Queue *q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

void enqueue(Queue *q, struct tcb *t) {
    if (q->size >= MAX_QUEUE_SIZE) {
        fprintf(stderr, "Error: Queue size exceeded MAX_QUEUE_SIZE (%d)\n", MAX_QUEUE_SIZE);
        return;
    }
    QueueNode *node = malloc(sizeof(QueueNode));
    node->tcb = t;
    node->next = NULL;
    if (q->tail == NULL) {
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->size++;
}

struct tcb *dequeue(Queue *q) {
    if (q->head == NULL) return NULL;
    QueueNode *temp = q->head;
    struct tcb *t = temp->tcb;
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    free(temp);
    q->size--;
    return t;
}
