#include "cebus/core/arena.h"

#include "cebus/core/debug.h"

typedef struct TestChunk {
  struct TestChunk *next, *prev;
  usize cap;
  usize allocated;
  u8 data[];
} TestChunk;

CONST_FN static usize test_align(usize size) {
  const usize mask = sizeof(void *) - 1;
  return (size + mask) & ~mask;
}

static void test_arena(void) {
  Arena arena = {0};

  const usize n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  cebus_assert(buffer, "Buffer was not allocated");
  cebus_assert(arena.begin, "Begin was not set");

  TestChunk *tc = (TestChunk *)arena.begin;
  cebus_assert(tc->allocated == test_align(n_bytes), "Not enough bytes are allocated");

  arena_free(&arena);
}

static void test_chunks(void) {
  Arena arena = {0};
  const usize n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  cebus_assert(buffer, "Buffer was not allocated");

  char *buffer2 = arena_alloc(&arena, n_bytes);
  cebus_assert(buffer + test_align(n_bytes) == buffer2, "'buffer2' was not next to 'buffer'");

  TestChunk *tc = (TestChunk *)arena.begin;
  const usize more_bytes = tc->cap;
  char *big_buffer = arena_alloc(&arena, more_bytes);
  cebus_assert(big_buffer, "Buffer was not allocated");

  TestChunk *tc2 = (TestChunk *)arena.begin;
  cebus_assert(tc2 != tc, "No new chunk was allocated");
  cebus_assert(tc2->allocated == more_bytes, "Not enough bytes are allocated");

  arena_free(&arena);
}

static void test_calloc(void) {
  Arena arena = {0};

  const usize n_bytes = 20;
  char *buffer = arena_calloc(&arena, n_bytes);
  for (usize i = 0; i < n_bytes; i++) {
    cebus_assert(buffer[i] == '\0', "Buffer was not zero initialized");
  }

  arena_free(&arena);
}

static void test_reset(void) { // NOLINT
  Arena arena = {0};

  int *chunk = arena_alloc_chunk(&arena, sizeof(int));

  const usize n_bytes = 10;
  char *buffer = arena_alloc(&arena, n_bytes);
  cebus_assert(buffer, "Buffer was not allocated");

  const usize more_bytes = ((TestChunk *)arena.begin)->cap;
  char *big_buffer = arena_alloc(&arena, more_bytes);
  cebus_assert(big_buffer, "Buffer was not allocated");

  arena_reset(&arena);

  TestChunk *tc = (TestChunk *)arena.begin;
  cebus_assert(tc->allocated == 0, "First Chunk was not reset");
  cebus_assert(tc->next->allocated == 0, "Second Chunk was not reset");

  char *buffer_after_reset = arena_alloc(&arena, n_bytes);
  cebus_assert(buffer_after_reset, "Buffer was not allocated");
  cebus_assert(tc->allocated == test_align(n_bytes), "Not enough bytes are allocated");

  char *big_buffer_after_reset = arena_alloc(&arena, more_bytes);
  cebus_assert(big_buffer_after_reset, "Buffer was not allocated");

  TestChunk *chunk_after_reset = (TestChunk *)((u8 *)chunk - sizeof(TestChunk));
  cebus_assert(chunk_after_reset->allocated == sizeof(int), "");

  arena_free(&arena);
}

static void test_size(void) {
  Arena arena = {0};

  int *i = arena_alloc(&arena, sizeof(*i));
  cebus_assert(arena_size(&arena) == test_align(sizeof(*i)), "size not matching: %zu",
               arena_size(&arena));
  cebus_assert(arena_real_size(&arena) == KILOBYTES(4), // NOLINT
               "chunk size does not match. did the default chunk size change?");

  arena_free(&arena);
}

int main(void) {
  test_arena();
  test_chunks();
  test_calloc();
  test_reset();
  test_size();
}
