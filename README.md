# Blua Diagnostics — Sistema de Fila de Atendimento

Sistema de gerenciamento de fila de teleconsultas da plataforma Blua (Care Plus), implementado em C com estruturas de dados construídas do zero sobre listas encadeadas.

## Vídeo
https://youtu.be/0jWBHbSoWlE

## Relatório do projeto
O relatório do projeto está em [relatorio.pdf](./relatorio.pdf) na raíz do repositório

## Estruturas implementadas

| Estrutura | Política | Uso no sistema |
|-----------|----------|----------------|
| **Fila** (Queue) | FIFO | Fila principal de pacientes aguardando atendimento |
| **Pilha** (Stack) | LIFO | Histórico das últimas 10 consultas finalizadas |
| **Lista Encadeada** | — | Base de ambas as estruturas acima |

Nenhuma biblioteca externa de fila/pilha foi utilizada. Todas as estruturas são construídas manualmente com alocação dinâmica de nós (`Node*`).

### Operações disponíveis

**Fila:** `enqueue`, `dequeue`, `is_queue_empty`, `print_queue`, `queue_prioritize`, `queue_cancel`

**Pilha:** `push`, `push_limited`, `pop`, `is_stack_empty`, `stack_size`, `print_stack`

## Como compilar e executar

### Executável (não recomendado)
Para facilitar a vida do professor criei um executável chamado blua_diagnostics.exe, mas o mais recomendado é compilar e executar o código

### Pré-requisitos

- GCC (ou qualquer compilador C11)
- CMake >= 3.10

### Compilar com CMake

```bash
cd dsa-sprint3
cmake -S . -B build
cmake --build build
./build/blua_diagnostics
```

### Compilar diretamente com GCC

```bash
cd dsa-sprint3
gcc -std=c11 -o blua_diagnostics main.c structures/linked_list.c structures/queue.c structures/stack.c
./blua_diagnostics
```

## Menu interativo

```
- n -> Novo Paciente
- a -> Atender Próximo Paciente
- r -> Priorizar paciente por ID
- c -> Cancelar paciente por ID
- l -> Limpar fila
- z -> Desfazer última ação
- p -> Exibir fila
- h -> Exibir histórico
- q -> Sair
```

## Cenários de uso

### Cenário 1 — Fluxo normal de teleconsulta (FIFO)

Três pacientes se cadastram e são atendidos na ordem de chegada.

```
n -> ID: 1, Nome: Ana Silva       # Ana entra na fila
n -> ID: 2, Nome: Bruno Lima      # Bruno entra atrás de Ana
n -> ID: 3, Nome: Carla Dias      # Carla entra por último
p                                 # Fila: [1] Ana → [2] Bruno → [3] Carla
a                                 # Ana é atendida (primeira a entrar)
a                                 # Bruno é atendido
h                                 # Histórico mostra Bruno no topo, Ana abaixo
```

### Cenário 2 — Priorização de emergência e cancelamento

Um paciente com situação crítica é movido para o início da fila; outro cancela o atendimento.

```
n -> ID: 10, Nome: Diego Ramos
n -> ID: 20, Nome: Eva Souza
n -> ID: 30, Nome: Fábio Nunes
r -> ID: 30                       # Fábio (emergência) vai para o início da fila
p                                 # Fila: [30] Fábio → [10] Diego → [20] Eva
c -> ID: 10                       # Diego cancela o atendimento
p                                 # Fila: [30] Fábio → [20] Eva
a                                 # Fábio é atendido primeiro
```

### Cenário 3 — Desfazer ações administrativas (Undo)

O operador comete enganos e usa o recurso de desfazer para corrigi-los.

```
n -> ID: 5, Nome: Gabriela Melo
n -> ID: 6, Nome: Hugo Ferreira
a                                 # Gabriela é atendida
z                                 # Undo: Gabriela é reinserida na frente da fila
p                                 # Fila: [5] Gabriela → [6] Hugo
r -> ID: 6                        # Hugo priorizado por engano
z                                 # Undo: Hugo retorna ao fim da fila
p                                 # Fila: [5] Gabriela → [6] Hugo
```

## Estrutura do projeto

```
dsa-sprint3/
├── main.c                    # Ponto de entrada, menu interativo e lógica de undo
├── CMakeLists.txt
├── blua_diagnostics.exe      # Executável
├── README.,d                 # Este arquivo :)
└── structures/
    ├── linked_list.h / .c    # Nó e operações primitivas de lista encadeada
    ├── queue.h / .c          # Fila FIFO com priorização e cancelamento
    └── stack.h / .c          # Pilha LIFO com limite de tamanho
```
