#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#define NAME_MAX_LEN 64

typedef struct Node {
  int data;
  char name[NAME_MAX_LEN];
  struct Node* next;
} Node;

Node* new_node(int value, const char* name);

void insert_begin(Node** list, int value, const char* name);

void insert_end(Node** list, int value, const char* name);

int rm_start(Node** list, char* name_out);

int rm_end(Node** list, char* name_out);

int is_empty(Node* list);

void print_linked_list(Node* list);

void clear_list(Node** list);

int rm_by_id(Node** list, int id, char* name_out);

int exists_by_id(Node* list, int id);

#endif