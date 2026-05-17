# Relatório Técnico | Sistema de Gerenciamento de Fila Hospitalar

**Disciplina:** Estruturas de Dados e Algoritmos
**Turma:** 1CCPR
**Grupo:**
* Pedro Ricardo de Almeida | 567056
* Agatha Carolina Rodrigues | 568352
* Arthur Berthi | 568017
* João Dyonisio Rocha | 567087
* Nicolas Natal | 568230

## 1. Escolha das Estruturas de Dados

O sistema combina quatro estruturas, cada uma selecionada pela adequação semântica ao problema que resolve.

### Lista Encadeada Simples — primitiva compartilhada

Base de todas as demais estruturas. Cada nó armazena `id`, `nome` e ponteiro `next`.
**Por quê:** tamanho dinâmico (número de pacientes é imprevisível), inserção/remoção nas extremidades em O(1) e suporte a remoção por ID arbitrário. Arrays exigiriam limite fixo e deslocamento O(n) na remoção do início.

### Fila FIFO — atendimento por ordem de chegada

Dois ponteiros `start` (frente) e `end` (último elemento) sobre a lista encadeada.
**Por quê:** a política FIFO espelha diretamente o critério de justiça hospitalar. O ponteiro `end` reduz `enqueue` de O(n) para **O(1)** — crítico em sistemas com alta taxa de chegada. Priorização e cancelamento usam `rm_by_id` + `fix_end` (O(n)), aceitos por serem operações raras.

### Pilha LIFO — histórico de atendidos (máx. 10)

Ponteiro `top` para o nó mais recente; campo `size` mantido incrementalmente.
**Por quê:** LIFO exibe o atendimento mais recente no topo, que é onde o operador espera encontrá-lo. O limite de 10 entradas é uma decisão de domínio — históricos longos têm utilidade decrescente. `push_limited` descarta o registro mais antigo quando cheia.

### Buffer Circular estático — log de undo (máx. 15 ações)

Array fixo com aritmética modular; sem alocação dinâmica.
**Por quê:** o limite de 15 ações é fixo e imutável. Arrays superam listas encadeadas nesse cenário: sem overhead de ponteiros por nó, sem fragmentação de heap, operações O(1) garantidas. A pilha encadeada seria desnecessariamente flexível para um limite estrito e pequeno.

## 2. Diagrama de Funcionamento

```
┌─────────────────────────────────────────────────────────────────┐
│                      SISTEMA DE FILA                            │
│  Entrada do usuário                                             │
│        │                                                        │
│        ▼                                                        │
│  ┌─────────────────────────────────────────────────────────┐    │
│  │                    MENU PRINCIPAL                       │    │
│  │  [n] Novo   [a] Atender  [r] Priorizar  [c] Cancelar    │    │
│  │  [l] Limpar [z] Desfazer [p] Exibir     [h] Histórico   │    │
│  └─────────────────────────────────────────────────────────┘    │
│        │                                                        │
│   ┌────┴──────────────────────────────────┐                     │
│   ▼                                       ▼                     │
│ ┌──────────────────────┐      ┌───────────────────────────┐     │
│ │        FILA          │      │     BUFFER CIRCULAR       │     │
│ │  (Lista Encadeada)   │◄─────│  (Log de ações / Undo)    │     │
│ │                      │      │  MAX_ACTIONS = 15         │     │
│ │  [n] enqueue → fim   │      └───────────────────────────┘     │
│ │  [a] dequeue ← ini   │                   ▲                    │
│ │  [r] rm_by_id +      │           log_action()                 │
│ │      insert_begin    │           pop_action()                 │
│ │  [c] rm_by_id        │                                        │
│ │  [l] clear_list      │                                        │
│ └──────────┬───────────┘                                        │
│            │ [a] paciente atendido                              │
│            ▼                                                    │
│ ┌──────────────────────┐                                        │
│ │        PILHA         │                                        │
│ │  (Lista Encadeada)   │                                        │
│ │  Histórico atendidos │                                        │
│ │  [h] print_stack     │                                        │
│ └──────────────────────┘                                        │
└─────────────────────────────────────────────────────────────────┘
```

## 3. Análise de Complexidade

| Operação | Complexidade | Observação |
|----------|-------|------------|
| `insert_begin` | **O(1)** | Atualiza apenas o ponteiro de cabeça |
| `insert_end` | O(n) | Percorre até o fim (sem ponteiro de cauda) |
| `rm_start` | **O(1)** | Desencadeia o nó de cabeça |
| `rm_end` | O(n) | Percorre até o penúltimo nó |
| `rm_by_id` | O(n) | Busca linear por identificador |
| `clear_list` | O(n) | n remoções do início |
| `enqueue` | **O(1)** | Ponteiro `end` elimina travessia |
| `dequeue` | **O(1)** | `rm_start` |
| `is_queue_empty` | **O(1)** | Verifica `start == NULL` |
| `queue_cancel` | O(n) | `rm_by_id` + `fix_end` |
| `queue_prioritize` | O(n) | `rm_by_id` + `fix_end` + `insert_begin` |
| `push` | **O(1)** | `insert_begin` |
| `push_limited` | O(n) pior caso | `rm_end` O(n) quando pilha cheia (máx. 10 nós) |
| `pop` | **O(1)** | `rm_start` |
| `stack_size` | **O(1)** | Campo `size` mantido incrementalmente | -- |
| `log_action` | **O(1)** — escrita indexada + aritmética modular | -- |
| `pop_action` | **O(1)** — leitura indexada + aritmética modular | -- |
| Undo `ENQUEUE` | O(n) | `rm_end` + recalcula `end` |
| Undo `DEQUEUE` | **O(1)** | `insert_begin` |
| Undo `PRIORITIZE` | **O(1)** | `rm_start` + `enqueue` |
| Undo `CANCEL` | **O(1)** | `enqueue` |
| Undo `CLEAR` | — | Não suportado, pois estado anterior descartado |

> OBS: O consumo de memória é linear no número de pacientes em espera — o mínimo teórico para qualquer sistema que precise armazenar todos eles. As estruturas auxiliares operam com espaço constante.