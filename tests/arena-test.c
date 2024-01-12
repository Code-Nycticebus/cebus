#include "core/arena.h"

#include "core/asserts.h"

typedef struct TestChunk {
  struct TestChunk *next;
  usize cap;
  usize allocated;
  u8 data[];
} TestChunk;

static void test_arena(void) {
  Arena arena = {0};

  const usize n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  clib_assert(buffer, "Buffer was not allocated");
  clib_assert(arena.begin, "Begin was not set");

  TestChunk *tc = (TestChunk *)arena.begin;
  clib_assert(tc->allocated == n_bytes, "Not enough bytes are allocated");

  arena_free(&arena);
}

static void test_chunks(void) {
  Arena arena = {0};
  const usize n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  clib_assert(buffer, "Buffer was not allocated");

  TestChunk *tc = (TestChunk *)arena.begin;
  clib_assert(tc->allocated == n_bytes, "Not enough bytes are allocated");

  const usize more_bytes = tc->cap;
  char *big_buffer = arena_alloc(&arena, more_bytes);
  clib_assert(big_buffer, "Buffer was not allocated");

  TestChunk *tc2 = (TestChunk *)arena.begin;
  clib_assert(tc2 != tc, "No new chunk was allocated");
  clib_assert(tc2->allocated == more_bytes, "Not enough bytes are allocated");

  arena_free(&arena);
}

static void test_calloc(void) {
  Arena arena = {0};

  const usize n_bytes = 20;
  char *buffer = arena_calloc(&arena, n_bytes);
  for (usize i = 0; i < n_bytes; i++) {
    clib_assert(buffer[i] == '\0', "Buffer was not zero initialized");
  }

  arena_free(&arena);
}

static void test_reset(void) { // NOLINT
  Arena arena = {0};

  const usize n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  clib_assert(buffer, "Buffer was not allocated");

  const usize more_bytes = ((TestChunk *)arena.begin)->cap;
  char *big_buffer = arena_alloc(&arena, more_bytes);
  clib_assert(big_buffer, "Buffer was not allocated");

  arena_reset(&arena);
  TestChunk *tc = (TestChunk *)arena.begin;
  clib_assert(tc->allocated == 0, "First Chunk was not reset");
  clib_assert(tc->next->allocated == 0, "Second Chunk was not reset");

  char *buffer_after_reset = arena_alloc(&arena, n_bytes);
  clib_assert(buffer_after_reset, "Buffer was not allocated");
  clib_assert(tc->allocated == n_bytes, "Not enough bytes are allocated");

  char *big_buffer_after_reset = arena_alloc(&arena, more_bytes);
  clib_assert(big_buffer_after_reset, "Buffer was not allocated");

  arena_free(&arena);
}

static void test_temp(void) {
  Arena arena = {0};

  void *data = arena_alloc_chunk(&arena, 10);

  clib_assert(data, "Should not be NULL");
  clib_assert(arena.begin, "Should not be NULL");

  TestChunk *chunk = (TestChunk *)arena.begin;
  clib_assert(chunk->allocated == chunk->cap, "Did not block the entire chunk");

  void *data2 = arena_alloc(&arena, 100);
  clib_assert(data2, "Should not be NULL");

  TestChunk *chunk2 = (TestChunk *)arena.begin;
  clib_assert(chunk2->allocated == 100, "Did not block the entire chunk");

  arena_free(&arena);
}

int main(void) {
  test_arena();
  test_chunks();
  test_calloc();
  test_reset();
  test_temp();
}
