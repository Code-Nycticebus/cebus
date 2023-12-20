#include "clib/arena.h"

#include "clib/da.h"

#include <assert.h>
#include <stdint.h>

typedef struct TestChunk {
  struct TestChunk *next;
  size_t cap;
  size_t allocated;
  uint8_t data[];
} TestChunk;

void test_arena(void) {
  Arena arena = {0};

  const size_t n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  assert(buffer);
  assert(arena.begin);

  TestChunk *tc = (TestChunk *)arena.begin;
  assert(tc->allocated == n_bytes);

  arena_free(&arena);
}

void test_chunks(void) {
  Arena arena = {0};
  const size_t n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  assert(buffer);

  TestChunk *tc = (TestChunk *)arena.begin;
  assert(tc->allocated == n_bytes);

  const size_t more_bytes = tc->cap;
  char *big_buffer = arena_alloc(&arena, more_bytes);
  assert(big_buffer);

  TestChunk *tc2 = (TestChunk *)arena.begin;
  assert(tc2 != tc);
  assert(tc->allocated == more_bytes);

  arena_free(&arena);
}

void test_calloc(void) {
  Arena arena = {0};

  const size_t n_bytes = 20;
  char *buffer = arena_calloc(&arena, n_bytes);
  for (size_t i = 0; i < n_bytes; i++) {
    assert(buffer[i] == '\0');
  }

  arena_free(&arena);
}

void test_reset(void) { // NOLINT
  Arena arena = {0};

  const size_t n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  assert(buffer);

  const size_t more_bytes = ((TestChunk *)arena.begin)->cap;
  char *big_buffer = arena_alloc(&arena, more_bytes);
  assert(big_buffer);

  arena_reset(&arena);
  TestChunk *tc = (TestChunk *)arena.begin;
  assert(tc->allocated == 0);
  assert(tc->next->allocated == 0);

  char *buffer_after_reset = arena_alloc(&arena, n_bytes);
  assert(buffer_after_reset);
  assert(tc->allocated == n_bytes);

  char *big_buffer_after_reset = arena_alloc(&arena, more_bytes);
  assert(big_buffer_after_reset);

  arena_free(&arena);
}

int main(void) {
  test_arena();
  test_calloc();
  test_reset();
}
