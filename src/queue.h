#pragma once

#include "board.h"

typedef struct queue_node queue_node;
struct queue_node {
    board *key;
    queue_node *next;
};

typedef struct {
    queue_node *front;
    queue_node *rear;
} queue;

queue *
create_queue();

void
enqueue(queue *q, board *key);

queue_node *
dequeue(queue *q);

void
destroy_queue(queue *q);
