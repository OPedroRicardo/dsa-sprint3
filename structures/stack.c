#include <stdio.h>
#include "stack.h"

void init_stack(Stack* p) {
  p->top = NULL;
  p->size = 0;
}

void push(Stack* p, int value, const char* name) {
  insert_begin(&(p->top), value, name);
  p->size++;
}

void push_limited(Stack* p, int value, const char* name, int max) {
  if (p->size >= max) {
    rm_end(&(p->top), NULL);
    p->size--;
  }
  insert_begin(&(p->top), value, name);
  p->size++;
}

int pop(Stack* p, char* name_out) {
  if (is_stack_empty(p)) return -1;
  p->size--;
  return rm_start(&(p->top), name_out);
}

int is_stack_empty(Stack* p) { return p->top == NULL; }

int stack_size(Stack* p) { return p->size; }

void print_stack(Stack* p) { printf("Topo -> "); print_linked_list(p->top); }