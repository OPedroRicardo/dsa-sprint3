#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

Node* new_node(int value, const char* name) {
  Node* new = (Node*) malloc(sizeof(Node));
  if (new != NULL) {
    new->data = value;
    strncpy(new->name, name, NAME_MAX_LEN - 1);
    new->name[NAME_MAX_LEN - 1] = '\0';
    new->next = NULL;
  }
  return new;
}

void insert_begin(Node** list, int value, const char* name) {
  Node* new = new_node(value, name);
  new->next = *list;
  *list = new;
}

void insert_end(Node** list, int value, const char* name) {
  Node* new = new_node(value, name);
  if (*list == NULL) {
    *list = new;
    return;
  }
  Node* current = *list;
  while (current->next != NULL) current = current->next;
  current->next = new;
}

// Remove do início. Se name_out != NULL, copia o nome para o caller.
int rm_start(Node** list, char* name_out) {
  if (*list == NULL) return -1;
  Node* temp = *list;
  int value = temp->data;
  if (name_out) strncpy(name_out, temp->name, NAME_MAX_LEN);
  *list = temp->next;
  free(temp);
  return value;
}

// Remove do fim. Se name_out != NULL, copia o nome para o caller.
int rm_end(Node** list, char* name_out) {
  if (*list == NULL) return -1;

  if ((*list)->next == NULL) {
    int value = (*list)->data;
    if (name_out) strncpy(name_out, (*list)->name, NAME_MAX_LEN);
    free(*list);
    *list = NULL;
    return value;
  }

  Node* current = *list;
  Node* last = NULL;
  while (current->next != NULL) {
    last = current;
    current = current->next;
  }
  last->next = NULL;
  int value = current->data;
  if (name_out) strncpy(name_out, current->name, NAME_MAX_LEN);
  free(current);
  return value;
}

int is_empty(Node* list) { return list == NULL; }

void print_linked_list(Node* list) {
  Node* current = list;
  while (current != NULL) {
    printf("[%d] %s -> ", current->data, current->name);
    current = current->next;
  }
  printf("NULL\n");
}

void clear_list(Node** list) {
  while (!is_empty(*list)) rm_start(list, NULL);
}

int rm_by_id(Node** list, int id, char* name_out) {
  if (*list == NULL) return -1;

  if ((*list)->data == id) return rm_start(list, name_out);

  Node* prev = *list;
  Node* cur = prev->next;
  while (cur != NULL) {
    if (cur->data == id) {
      prev->next = cur->next;
      if (name_out) strncpy(name_out, cur->name, NAME_MAX_LEN);
      free(cur);
      return id;
    }
    prev = cur;
    cur = cur->next;
  }
  return -1;
}

int exists_by_id(Node* list, int id) {
  Node* cur = list;
  while (cur != NULL) {
    if (cur->data == id) return 1;
    cur = cur->next;
  }
  return 0;
}