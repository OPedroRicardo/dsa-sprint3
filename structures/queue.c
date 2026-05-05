#include <stdio.h>
#include "queue.h"

void init_queue(Queue* q) {
  q->start = NULL;
  q->end = NULL;
}

void enqueue(Queue* q, int value, const char* name) {
  Node* new = new_node(value, name);

  if (is_queue_empty(q)) {
    q->start = new;
    q->end = new;
    return;
  }

  q->end->next = new;
  q->end = new;
}

int dequeue(Queue* q, char* name_out) {
  if (is_queue_empty(q)) {
    printf("Fila vazia!\n");
    return -1;
  }

  int value = rm_start(&(q->start), name_out);
  if (q->start == NULL) q->end = NULL;
  return value;
}

int is_queue_empty(Queue* q) { return q->start == NULL; }

void print_queue(Queue* q) { printf("Início -> "); print_linked_list(q->start); }

static void fix_end(Queue* q) {
  if (q->start == NULL) {
    q->end = NULL;
    return;
  }
  Node* cur = q->start;
  while (cur->next != NULL) cur = cur->next;
  q->end = cur;
}

int queue_prioritize(Queue* q, int id, char* name_out) {
  if (rm_by_id(&(q->start), id, name_out) == -1) return 0;
  fix_end(q);
  insert_begin(&(q->start), id, name_out);
  if (q->end == NULL) q->end = q->start;
  return 1;
}

int queue_cancel(Queue* q, int id, char* name_out) {
  if (rm_by_id(&(q->start), id, name_out) == -1) return 0;
  fix_end(q);
  return 1;
}