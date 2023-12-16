#include "clib/arena.h"

#include "clib/da.h"

#include <assert.h>
#include <stdint.h>

typedef struct {
  size_t cap;
  size_t allocated;
  uint8_t data[];
} TestChunk;

typedef struct {
  DA(TestChunk *) chunks;
} TestArena;

void test_arena(void) {
  Arena *arena = arena_make();
  TestArena *ta = (TestArena *)arena;
  assert(ta);

  const size_t n_bytes = 10;
  char *buffer = arena_alloc(arena, n_bytes);
  assert(buffer);

  assert(ta->chunks.items[0]->allocated == n_bytes);

  arena_free(arena);
}

void test_chunks(void) {
  Arena *arena = arena_make();
  TestArena *ta = (TestArena *)arena;
  const size_t n_bytes = 10;
  char *buffer = arena_alloc(arena, n_bytes);
  assert(buffer);
  assert(ta->chunks.items[0]->allocated == n_bytes);

  const size_t more_bytes = ta->chunks.items[0]->cap;
  char *big_buffer = arena_alloc(arena, more_bytes);
  assert(big_buffer);

  assert(1 < ta->chunks.len);
  assert(ta->chunks.items[1]->allocated == more_bytes);

  arena_free(arena);
}

void test_calloc(void) {
  Arena *arena = arena_make();
  assert(arena);

  const size_t n_bytes = 20;
  char *buffer = arena_calloc(arena, n_bytes);
  for (size_t i = 0; i < n_bytes; i++) {
    assert(buffer[i] == '\0');
  }
}

int main(void) {
  test_arena();
  test_calloc();
}
