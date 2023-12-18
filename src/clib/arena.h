#ifndef __CLIB_ARENA_H__
#define __CLIB_ARENA_H__

#include "defines.h"

// TODO make arena be static one. maybe no initialization needed.
// 	Maybe the linked list approach would be better?


typedef struct Arena Arena;

Arena *arena_make(void);
void arena_free(Arena *arena);

void *arena_alloc(Arena *arena, size_t size);
void *arena_calloc(Arena *arena, size_t size);

#endif // !__CLIB_ARENA_H__
