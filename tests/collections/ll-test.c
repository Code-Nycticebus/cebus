#include <stddef.h>

typedef struct LLNode {
  struct LLNode *next, *prev;
} LLNode;

typedef struct {
  LLNode *begin, *end;
} LinkedList;

void ll_append(LinkedList *ll, LLNode *node);

#define ll_for_each(T, ll, next)                                               \
  for (T *next = (T *)(ll)->begin; next; next = (T *)next->head.next)

/////////////////////////////////////////////////////////////

void ll_append(LinkedList *ll, LLNode *node) {
  if (ll->begin == NULL) {
    ll->begin = node;
  }
  if (ll->end != NULL) {
    node->prev = ll->end;
    ll->end->next = node;
  }
  ll->end = node;
}

/////////////////////////////////////////////////////////////

#include "clib/core/arena.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  LLNode head;
  int value;
} IntNode;

static LLNode *int_node_init(int value, Arena *arena) {
  IntNode *node = arena_calloc(arena, sizeof(IntNode));
  node->value = value;
  return (LLNode *)node;
}

int main(void) {
  Arena arena = {0};

  LinkedList ll = {0};

  for (usize i = 0; i < 10; ++i) {
    ll_append(&ll, int_node_init(rand() % 255, &arena));
  }

  printf("{");
  ll_for_each(IntNode, &ll, next) {
    if (next != (void *)ll.begin) {
      printf(", ");
    }
    printf("%d", next->value);
  }
  printf("}\n");

  arena_free(&arena);
}
