#ifndef __CLIB_ARENA_H__
#define __CLIB_ARENA_H__

#include "defines.h"

typedef struct Chunk Chunk;

typedef struct {
  Chunk *begin;
} Arena;

void arena_free(Arena *arena);

void *arena_alloc(Arena *arena, size_t size);
void *arena_calloc(Arena *arena, size_t size);

#endif // !__CLIB_ARENA_H__
