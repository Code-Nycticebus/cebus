#ifndef __CLIB_ARENA_H__
#define __CLIB_ARENA_H__

#include "defines.h"

typedef struct Chunk Chunk;

typedef struct {
  Chunk *begin;
} Arena;

void arena_free(Arena *arena);

void *arena_alloc(Arena *arena, usize size);
void *arena_calloc(Arena *arena, usize size);
void arena_reset(Arena *arena);

#endif /* !__CLIB_ARENA_H__ */
