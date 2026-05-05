#ifndef QUEUE_H
#define QUEUE_H
#include "linked_list.h"

typedef struct {
  Node* start;
  Node* end;
} Queue;

void init_queue(Queue* q);

void enqueue(Queue* q, int value, const char* name);

int dequeue(Queue* q, char* name_out);

int is_queue_empty(Queue* q);

void print_queue(Queue* q);

int queue_prioritize(Queue* q, int id, char* name_out);

int queue_cancel(Queue* q, int id, char* name_out);

#endif