#include "queue.h"
#include "board.h"

#include <stdlib.h>

queue *
create_queue() {
    queue *q = (queue *) malloc(sizeof(*q));
    q->front = NULL;
    q->rear = NULL;

    return q;
}

static queue_node *
create_queue_node(board *k) {
    queue_node *temp = (queue_node *) malloc(sizeof(*temp));
    temp->key = k;
    temp->next = NULL;

    return temp;
}

void
enqueue(queue *q, board *key) {
    queue_node *temp = create_queue_node(key);

    if (q->rear == NULL) {
        q->front = temp;
        q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

queue_node *
dequeue(queue *q) {
    if (q->front == NULL) {
        return NULL;
    }

    queue_node *temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }
    return temp;
}

void
destroy_queue(queue *q) {
    if (q) {
        while (q->front != NULL) {
            queue_node *current = dequeue(q);
            if (current) {
                destroy_board(current->key);
            }
            free(current);
        }
    }
    free(q);
}
