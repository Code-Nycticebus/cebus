#ifndef __CLIB_ARENA_H__
#define __CLIB_ARENA_H__

#include "core/defines.h"

typedef struct Chunk Chunk;

typedef struct {
  Chunk *begin;
} Arena;

void arena_free(Arena *arena);

void *arena_alloc(Arena *arena, usize size);
void *arena_calloc(Arena *arena, usize size);
void arena_reset(Arena *arena);

void *arena_alloc_chunk(Arena *arena, usize size);
void *arena_calloc_chunk(Arena *arena, usize size);
void *arena_realloc_chunk(Arena *arena, void *ptr, usize size);
void arena_free_chunk(Arena *arena, void *ptr);

#endif /* !__CLIB_ARENA_H__ */
