#include "core/arena.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct LLNode {
  struct LLNode *next, *prev;
} LLNode;

#define LL_NODE(N) (&(N)->node)
#define LL_VALUE(N, node) (&((N *)(node))->value)

#define NODE(T)                                                                \
  struct {                                                                     \
    LLNode node;                                                               \
    T value;                                                                   \
  }

typedef struct {
  LLNode *begin, *end;
} LinkedList;

static void ll_append(LinkedList *ll, LLNode *node) {
  if (ll->begin == NULL) {
    ll->begin = node;
  }
  if (ll->end != NULL) {
    ll->end->next = node;
  }
  node->prev = ll->end;
  ll->end = node;
}

typedef NODE(int) IntNode;

static IntNode *cn_init(Arena *arena, int value) {
  IntNode *node = arena_calloc(arena, sizeof(IntNode));
  node->value = value;
  return node;
}

int main(void) {
  Arena arena = {0};
  LinkedList ll = {0};

  ll_append(&ll, LL_NODE(cn_init(&arena, 420)));
  ll_append(&ll, LL_NODE(cn_init(&arena, 69)));
  ll_append(&ll, LL_NODE(cn_init(&arena, 23)));
  ll_append(&ll, LL_NODE(cn_init(&arena, 31)));

  for (LLNode *next = ll.begin; next != NULL; next = next->next) {
    int *node = LL_VALUE(IntNode, next);
    printf("%d\n", *node);
  }

  arena_free(&arena);
}
