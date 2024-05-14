/* DOCUMENTATION
## Initialization

To start using the library, initialize an `Arena` struct:

```c
Arena arena = {0};
```

## Memory Allocation

Allocate memory from the arena using `arena_alloc` or `arena_calloc` for
uninitialized or zero-initialized memory, respectively:

```c
int* i1 = arena_alloc(&arena, sizeof(int));
int* i2 = arena_alloc(&arena, sizeof(int));
int* i3 = arena_alloc(&arena, sizeof(int));
```

## Memory Deallocation

Deallocate all memory associated with an arena at once using `arena_free`. This
operation frees all memory chunks allocated from the arena, providing a
significant performance advantage over individual deallocation:

```c
arena_free(&arena);
```

## Chunk

The library also provides functions for more granular control over memory chunks
within an arena:

- `arena_alloc_chunk`: Allocate a new chunk of memory.
- `arena_calloc_chunk`: Allocates a new, zero initialized, chunk of memory.
- `arena_realloc_chunk`: Reallocate a previously allocated chunk to a new size.
- `arena_free_chunk`: Free a specific chunk of memory (advanced use cases).
*/

#ifndef __CEBUS_ARENA_H__
#define __CEBUS_ARENA_H__

#include "cebus/core/defines.h"

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

#endif /* !__CEBUS_ARENA_H__ */
