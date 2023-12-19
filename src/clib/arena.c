#include "arena.h"

#include <stdlib.h>
#include <string.h>

#define CHUNK_DEFAULT_SIZE KILOBYTES(8)

struct Chunk {
  Chunk *next;
  size_t cap;
  size_t allocated;
  uint8_t data[];
};

static Chunk *chunk_allocate(size_t size) {
  Chunk *chunk = malloc(sizeof(Chunk) + size);
  if (chunk == NULL) {
    return NULL;
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

void *arena_alloc(Arena *arena, size_t size) {
  Chunk *chunk = arena->begin;
  if (chunk == NULL || !(chunk->allocated + size < chunk->cap)) {
    const size_t chunk_size =
        size < CHUNK_DEFAULT_SIZE ? CHUNK_DEFAULT_SIZE : size;
    chunk = chunk_allocate(chunk_size);
    chunk->next = arena->begin;
    arena->begin = chunk;
  }
  void *ptr = &chunk->data[chunk->allocated];
  chunk->allocated += size;
  return ptr;
}

void *arena_calloc(Arena *arena, size_t size) {
  void *ptr = arena_alloc(arena, size);
  memset(ptr, 0, size);
  return ptr;
}
