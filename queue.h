#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE_SIZE 100

struct tcb;

typedef struct queue_node {
    struct tcb *tcb;
    struct queue_node *next;
} QueueNode;

typedef struct {
    QueueNode *head;
    QueueNode *tail;
    int size;
} Queue;

void queue_init(Queue *q);
void enqueue(Queue *q, struct tcb *t);
struct tcb *dequeue(Queue *q);

#endif