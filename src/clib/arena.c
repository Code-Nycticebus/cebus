#include "arena.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"

#define CHUNK_DEFAULT_SIZE KILOBYTES(8)

struct Chunk {
  Chunk *next;
  usize cap;
  usize allocated;
  u8 data[];
};

static Chunk *chunk_allocate(usize size) {
  Chunk *chunk = malloc(sizeof(Chunk) + size);
  if (chunk == NULL) {
    clib_log_fatal("Memory allocation failed: %s", strerror(errno));
    abort();
  }
  chunk->cap = size;
  chunk->allocated = 0;
  return chunk;
}

static void chunk_free(Chunk *chunk) { free(chunk); }

void arena_free(Arena *arena) {
  Chunk *next = arena->begin;
  while (next != NULL) {
    Chunk *temp = next;
    next = next->next;
    chunk_free(temp);
  }
  arena->begin = NULL;
}

void arena_reset(Arena *arena) {
  for (Chunk *next = arena->begin; next != NULL; next = next->next) {
    next->allocated = 0;
  }
}

void *arena_alloc(Arena *arena, usize size) {
  Chunk *chunk = arena->begin;
  for (; chunk != NULL; chunk = chunk->next) {
    clib_assert_debug(size <= SIZE_MAX - chunk->allocated, "integer overflow");
    if (chunk->allocated + size < chunk->cap) {
      break;
    }
  }
  if (chunk == NULL) {
    const usize chunk_size =
        size < CHUNK_DEFAULT_SIZE ? CHUNK_DEFAULT_SIZE : size;
    chunk = chunk_allocate(chunk_size);
    chunk->next = arena->begin;
    arena->begin = chunk;
  }
  void *ptr = &chunk->data[chunk->allocated];
  chunk->allocated += size;
  return ptr;
}

void *arena_calloc(Arena *arena, usize size) {
  void *ptr = arena_alloc(arena, size);
  memset(ptr, 0, size);
  return ptr;
}
