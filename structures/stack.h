#ifndef STACK_H
#define STACK_H
#include "linked_list.h"

typedef struct {
  Node* top;
  int size;
} Stack;

void init_stack(Stack* p);

void push(Stack* p, int value, const char* name);

void push_limited(Stack* p, int value, const char* name, int max);

int pop(Stack* p, char* name_out);

int is_stack_empty(Stack* p);

int stack_size(Stack* p);

void print_stack(Stack* p);

#endif