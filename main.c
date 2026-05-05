#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "structures/queue.h"
#include "structures/stack.h"

#define MAX_HISTORY 10
#define MAX_ACTIONS 15

#define ACTION_ENQUEUE    1
#define ACTION_DEQUEUE    2
#define ACTION_CLEAR      3
#define ACTION_PRIORITIZE 4
#define ACTION_CANCEL     5

typedef struct {
  int type;
  int value;
  char name[NAME_MAX_LEN];
} ActionEntry;

ActionEntry action_log[MAX_ACTIONS];
int action_log_head = 0;
int action_log_count = 0;

void log_action(int type, int value, const char* name) {
  action_log[action_log_head].type = type;
  action_log[action_log_head].value = value;
  strncpy(action_log[action_log_head].name, name ? name : "", NAME_MAX_LEN);
  action_log_head = (action_log_head + 1) % MAX_ACTIONS;
  if (action_log_count < MAX_ACTIONS) action_log_count++;
}

int pop_action(ActionEntry* out) {
  if (action_log_count == 0) return 0;
  action_log_count--;
  action_log_head = (action_log_head - 1 + MAX_ACTIONS) % MAX_ACTIONS;
  *out = action_log[action_log_head];
  return 1;
}

int read_positive_int(const char* prompt) {
  int v;
  while (1) {
    printf("%s", prompt);
    if (scanf("%d", &v) == 1 && v > 0) return v;
    printf("  Inválido. Digite um número inteiro positivo.\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
  }
}

void read_non_empty_string(const char* prompt, char* out, int max_len) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
  while (1) {
    printf("%s", prompt);
    if (fgets(out, max_len, stdin) == NULL) continue;
    out[strcspn(out, "\n")] = '\0';
    int i = 0;
    while (out[i] && isspace((unsigned char)out[i])) i++;
    if (out[i] != '\0') return;
    printf("  Inválido. O nome não pode ser vazio.\n");
  }
}

void print_options() {
  printf("\n------------------------------\n");
  printf("- n -> Novo Paciente\n");
  printf("- a -> Atender Próximo Paciente\n");
  printf("- r -> Priorizar paciente por ID\n");
  printf("- c -> Cancelar paciente por ID\n");
  printf("- l -> Limpar fila\n");
  printf("- z -> Desfazer última ação\n");
  printf("- p -> Exibir fila\n");
  printf("- h -> Exibir histórico\n");
  printf("- q -> Sair\n");
  printf("------------------------------\n");
}

void cmd_novo(Queue* queue) {
  int v;
  while (1) {
    v = read_positive_int("Número do paciente: ");
    if (!exists_by_id(queue->start, v)) break;
    printf("  Inválido. ID %d já está na fila.\n", v);
  }
  char name[NAME_MAX_LEN];
  read_non_empty_string("Nome do paciente: ", name, NAME_MAX_LEN);

  enqueue(queue, v, name);
  log_action(ACTION_ENQUEUE, v, name);
  printf("Paciente [%d] %s adicionado à fila.\n", v, name);
}

void cmd_atender(Queue* queue, Stack* history) {
  if (is_queue_empty(queue)) {
    printf("Fila vazia!\n");
    return;
  }
  char name[NAME_MAX_LEN];
  int v = dequeue(queue, name);
  push_limited(history, v, name, MAX_HISTORY);
  log_action(ACTION_DEQUEUE, v, name);
  printf("Paciente [%d] %s atendido e removido da fila.\n", v, name);
}

void cmd_priorizar(Queue* queue) {
  if (is_queue_empty(queue)) {
    printf("Fila vazia!\n");
    return;
  }
  int id = read_positive_int("ID do paciente a priorizar: ");
  char name[NAME_MAX_LEN];
  if (!queue_prioritize(queue, id, name)) {
    printf("Paciente com ID %d não encontrado na fila.\n", id);
    return;
  }
  log_action(ACTION_PRIORITIZE, id, name);
  printf("Paciente [%d] %s movido para o início da fila.\n", id, name);
}

void cmd_cancelar(Queue* queue) {
  if (is_queue_empty(queue)) {
    printf("Fila vazia!\n");
    return;
  }
  int id = read_positive_int("ID do paciente a cancelar: ");
  char name[NAME_MAX_LEN];
  if (!queue_cancel(queue, id, name)) {
    printf("Paciente com ID %d não encontrado na fila.\n", id);
    return;
  }
  log_action(ACTION_CANCEL, id, name);
  printf("Paciente [%d] %s removido da fila.\n", id, name);
}

void cmd_undo(Queue* queue) {
  ActionEntry entry;
  if (!pop_action(&entry)) {
    printf("Nenhuma ação para desfazer.\n");
    return;
  }

  if (entry.type == ACTION_ENQUEUE) {
    char name[NAME_MAX_LEN];
    rm_end(&(queue->start), name);
    if (queue->start == NULL) queue->end = NULL;
    else {
      Node* cur = queue->start;
      while (cur->next != NULL) cur = cur->next;
      queue->end = cur;
    }
    printf("Undo: paciente [%d] %s removido da fila.\n", entry.value, entry.name);
    return;
  }

  if (entry.type == ACTION_DEQUEUE) {
    insert_begin(&(queue->start), entry.value, entry.name);
    if (queue->end == NULL) queue->end = queue->start;
    printf("Undo: paciente [%d] %s reinserido na frente da fila.\n", entry.value, entry.name);
    return;
  }

  if (entry.type == ACTION_PRIORITIZE) {
    char name[NAME_MAX_LEN];
    int v = rm_start(&(queue->start), name);
    if (queue->start == NULL) queue->end = NULL;
    enqueue(queue, v, name);
    printf("Undo: paciente [%d] %s retornou ao fim da fila.\n", entry.value, entry.name);
    return;
  }

  if (entry.type == ACTION_CANCEL) {
    enqueue(queue, entry.value, entry.name);
    printf("Undo: paciente [%d] %s reinserido no fim da fila.\n", entry.value, entry.name);
    return;
  }

  if (entry.type == ACTION_CLEAR) {
    printf("Undo de 'limpar fila' não é suportado.\n");
  }
}

int main() {
  Queue queue;
  init_queue(&queue);

  Stack history;
  init_stack(&history);

  while (1) {
    char key;
    print_options();
    printf("Digite o comando: ");
    scanf(" %c", &key);

    if (key == 'q') break;

    if (key == 'n') { cmd_novo(&queue); continue; }
    if (key == 'a') { cmd_atender(&queue, &history); continue; }
    if (key == 'r') { cmd_priorizar(&queue); continue; }
    if (key == 'c') { cmd_cancelar(&queue); continue; }
    if (key == 'z') { cmd_undo(&queue); continue; }

    if (key == 'l') {
      if (is_queue_empty(&queue)) { printf("Fila já está vazia.\n"); continue; }
      log_action(ACTION_CLEAR, 0, "");
      clear_list(&(queue.start));
      init_queue(&queue);
      printf("Fila limpa.\n");
      continue;
    }

    if (key == 'p') {
      if (is_queue_empty(&queue)) { printf("Fila vazia.\n"); continue; }
      printf("Fila atual: ");
      print_queue(&queue);
      continue;
    }

    if (key == 'h') {
      if (is_stack_empty(&history)) { printf("Histórico vazio.\n"); continue; }
      printf("Histórico de pacientes atendidos (mais recente no topo):\n");
      print_stack(&history);
      printf("Total: %d/%d\n", stack_size(&history), MAX_HISTORY);
      continue;
    }

    printf("Comando não encontrado!\n");
  }

  clear_list(&(queue.start));
  clear_list(&(history.top));

  return 0;
}