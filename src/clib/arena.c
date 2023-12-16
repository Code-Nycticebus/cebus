#include "arena.h"

#include "da.h"

#include <stdlib.h>
#include <string.h>

#define CHUNK_DEFAULT_SIZE KILOBYTES(8)

typedef struct {
  size_t cap;
  size_t allocated;
  uint8_t data[];
} Chunk;

struct Arena {
  DA(Chunk *) chunks;
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
  da_init(&arena->chunks, 1);
  return arena;
}

void arena_free(Arena *arena) {
  for (size_t i = 0; i < arena->chunks.len; i++) {
    chunk_free(arena->chunks.items[i]);
  }
  da_free(&arena->chunks);
  free(arena);
}

void *arena_alloc(Arena *arena, size_t size) {
  Chunk *chunk = da_empty(&arena->chunks) ? NULL : da_last(&arena->chunks);
  if (chunk == NULL || !(chunk->allocated + size < chunk->cap)) {
    const size_t chunk_size =
        size < CHUNK_DEFAULT_SIZE ? CHUNK_DEFAULT_SIZE : size;
    da_push(&arena->chunks, chunk_allocate(chunk_size));
    chunk = da_last(&arena->chunks);
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
