#include "arena.h"

#include "asserts.h"
#include "type/integer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

#define CHUNK_DEFAULT_SIZE KILOBYTES(8)

struct Chunk {
  Chunk *next, *prev;
  usize cap;
  usize allocated;
  u8 data[];
};

////////////////////////////////////////////////////////////////////////////

static Chunk *chunk_allocate(usize size) {
  Chunk *chunk = malloc(sizeof(Chunk) + size);
  clib_assert(chunk != NULL, "Memory allocation failed: %s", strerror(errno));
  chunk->cap = size;
  chunk->allocated = 0;
  chunk->next = chunk->prev = 0;
  return chunk;
}

static void chunk_free(Chunk *chunk) { free(chunk); }

CONST static usize align(usize size) {
  const usize mask = sizeof(void *) - 1;
  return (size + mask) & ~mask;
}

////////////////////////////////////////////////////////////////////////////

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
  size = align(size);
  Chunk *chunk = arena->begin;
  for (; chunk != NULL; chunk = chunk->next) {
    clib_assert_debug(size <= SIZE_MAX - chunk->allocated, "integer overflow");
    if (chunk->allocated + size < chunk->cap) {
      break;
    }
  }
  if (chunk == NULL) {
    const usize chunk_size = usize_max(size, CHUNK_DEFAULT_SIZE);
    chunk = chunk_allocate(chunk_size);
    chunk->next = arena->begin;
    if (arena->begin) {
      arena->begin->prev = chunk;
    }
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

////////////////////////////////////////////////////////////////////////////

void *arena_alloc_chunk(Arena *arena, usize size) {
  usize chunk_size = usize_max(size, CHUNK_DEFAULT_SIZE);
  Chunk *chunk = chunk_allocate(chunk_size);
  chunk->allocated = chunk_size;
  chunk->next = arena->begin;
  if (arena->begin) {
    arena->begin->prev = chunk;
  }
  arena->begin = chunk;
  return chunk->data;
}

void *arena_calloc_chunk(Arena *arena, usize size) {
  void *data = arena_alloc_chunk(arena, size);
  memset(data, 0, size);
  return data;
}

void *arena_realloc_chunk(Arena *arena, void *ptr, usize size) {
  if (ptr == NULL) {
    return arena_alloc_chunk(arena, size);
  }
  Chunk *chunk = (Chunk *)((u8 *)ptr - sizeof(Chunk));
  if (size < chunk->allocated) {
    return chunk->data;
  }
  Chunk *new_chunk = realloc(chunk, sizeof(Chunk) + size);
  return new_chunk->data;
}

void arena_free_chunk(Arena *arena, void *ptr) {
  if (ptr == NULL) {
    return;
  }
  Chunk *chunk = (Chunk *)((u8 *)ptr - sizeof(Chunk));
  if (chunk == arena->begin) {
    arena->begin = chunk->next;
  }
  if (chunk->prev) {
    chunk->prev->next = chunk->next;
  }
  if (chunk->next) {
    chunk->next->prev = chunk->prev;
  }
  free(chunk);
}

////////////////////////////////////////////////////////////////////////////
