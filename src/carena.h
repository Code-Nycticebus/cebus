#ifndef __CARENA_H__
#define __CARENA_H__

#include <stddef.h>

typedef struct Arena Arena;

Arena *arena_make(void);
void arena_free(Arena *arena);

void *arena_alloc(Arena *arena, size_t size);
void *arena_calloc(Arena *arena, size_t size);

#endif // !__CARENA_H__
