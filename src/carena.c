#include "carena.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KILOBYTES(s) ((s)*1000)
#define MEGABYTES(s) ((s)*1e+6)

#define CHUNK_DEFAULT_SIZE KILOBYTES(1)

typedef struct {
  size_t cap;
  size_t allocated;
  uint8_t data[];
} Chunk;

struct Arena {
  size_t chunk_count;
  Chunk **chunk;
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

Arena *arena_make(void) {
  Arena *arena = malloc(sizeof(Arena));
  arena->chunk_count = 1;
  arena->chunk = malloc(sizeof(Chunk));
  arena->chunk[0] = chunk_allocate(CHUNK_DEFAULT_SIZE);
  return arena;
}

void arena_free(Arena *arena) {
  for (size_t i = 0; i < arena->chunk_count; i++) {
    chunk_free(arena->chunk[i]);
  }
  free(arena->chunk);
  free(arena);
}

void *arena_alloc(Arena *arena, size_t size) {
  Chunk *chunk = arena->chunk[arena->chunk_count - 1];
  if (!(chunk->allocated + size < chunk->cap)) {
    const size_t chunk_idx = arena->chunk_count++;
    arena->chunk = realloc(arena->chunk, arena->chunk_count * sizeof(Chunk));
    const size_t chunk_size =
        size < CHUNK_DEFAULT_SIZE ? CHUNK_DEFAULT_SIZE : size;
    arena->chunk[chunk_idx] = chunk_allocate(chunk_size);
    chunk = arena->chunk[chunk_idx];
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
