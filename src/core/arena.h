/* DOCUMENTATION
### Usage
Create a new Arena with:
```c
Arena arena = {0};
```

Now you can allocate from this arena.
```c
int* i1 = arena_alloc(&arena, sizeof(int));
int* i2 = arena_alloc(&arena, sizeof(int));
int* i3 = arena_alloc(&arena, sizeof(int));
```

Don't forget to free the arena once you're done. This frees all allocated
integers at once.
```c
arena_free(&arena);
```
*/

#ifndef __CLIB_ARENA_H__
#define __CLIB_ARENA_H__

#include "core/defines.h"

typedef struct Chunk Chunk;

typedef struct {
  Chunk *begin;
} Arena;

////////////////////////////////////////////////////////////////////////////

void arena_free(Arena *arena);

void *arena_alloc(Arena *arena, usize size);
void *arena_calloc(Arena *arena, usize size);
void arena_reset(Arena *arena);

////////////////////////////////////////////////////////////////////////////

void *arena_alloc_chunk(Arena *arena, usize size);
void *arena_calloc_chunk(Arena *arena, usize size);
void *arena_realloc_chunk(Arena *arena, void *ptr, usize size);
void arena_free_chunk(Arena *arena, void *ptr);

////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_ARENA_H__ */
