/* DOCUMENTATION
My HashMap takes a unique approach: it stores only the hashes of keys, not the
keys themselves. Most of the time, you don’t really need the original keys
hanging around. If you find yourself in a situation where you do, just pair it
with a dynamic array to cover those bases. See
[this](https://github.com/Code-Nycticebus/clib/blob/main/examples/word.c)
example.

As for the values, the HashMap is set up to work with simple, primitive
data types. You can use pointers to handle more complex values. But make sure
they have the same lifetime as the `HashMap`.

## Initialization

Creating a new `HashMap` involves initializing an `Arena`, then calling
`hm_create` or `hm_with_size` to initialize the hashmap with an optional initial
size:

```c
Arena arena = {0};
HashMap* hm = hm_create(&arena);
```

## HashMap Operations

Basic hashmap management includes clearing, copying, resizing, reserving
capacity, and updating from another hashmap:

- `hm_clear`: Clears the hashmap.
- `hm_copy`: Creates a copy of the hashmap.
- `hm_resize`: Resizes the hashmap. Used for preallocating space
- `hm_reserve`: Reserves space in the hashmap. Used before adding multiple
elements.
- `hm_update`: Merges another hashmap into the current one.

## Inserting Elements

Elements of various types can be inserted into the hashmap, including integers,
floating-point numbers, and pointers:

- `hm_insert_f32`, `hm_insert_f64`: Insert floating-point values.
- `hm_insert_i32`, `hm_insert_u32`, `hm_insert_i64`, `hm_insert_u64`: Insert
integer values.
- `hm_insert_mut_ptr`, `hm_insert_ptr`: Insert mutable or constant pointers.

## Querying Elements

Retrieve pointers to the values stored in the hashmap by their key hashes,
allowing for mutable or immutable access:

> :warning: Avoid storing pointers from the hashmap for extended periods.
> Keeping these pointers beyond the immediate scope can lead to undefined
behavior, as the underlying storage may change.

- `hm_get_f32_mut`, `hm_get_f64_mut`, etc.: Retrieve mutable pointers to values.
- `hm_get_f32`, `hm_get_f64`, etc.: Retrieve immutable pointers to values.
*/

#ifndef __CLIB_HASHMAP_H__
#define __CLIB_HASHMAP_H__

#include "clib/core/arena.h"
#include "clib/core/defines.h"

typedef struct HashMap HashMap;

///////////////////////////////////////////////////////////////////////////////

HashMap *hm_create(Arena *arena);
HashMap *hm_with_size(Arena *arena, usize size);

void hm_clear(HashMap *hm);

HashMap *hm_copy(HashMap *hm, Arena *arena);

void hm_resize(HashMap *hm, usize size);
void hm_reserve(HashMap *hm, usize size);

void hm_update(HashMap *hm, HashMap *other);

bool hm_remove(HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#define HM_TYPES(DO)                                                           \
  DO(f32)                                                                      \
  DO(f64)                                                                      \
  DO(i8)                                                                       \
  DO(u8)                                                                       \
  DO(i16)                                                                      \
  DO(u16)                                                                      \
  DO(i32)                                                                      \
  DO(u32)                                                                      \
  DO(i64)                                                                      \
  DO(u64)                                                                      \
  DO(usize)

#define HM_INSERT_DECL(T) bool hm_insert_##T(HashMap *hm, u64 hash, T value);
HM_TYPES(HM_INSERT_DECL)
#undef HM_INSERT_DECL

bool hm_insert_mut_ptr(HashMap *hm, u64 hash, void *value);
bool hm_insert_ptr(HashMap *hm, u64 hash, const void *value);

///////////////////////////////////////////////////////////////////////////////

#define HM_GET_MUT_DECL(T) T *hm_get_##T##_mut(const HashMap *hm, u64 hash);
HM_TYPES(HM_GET_MUT_DECL)
#undef HM_GET_MUT_DECL

void **hm_get_ptr_mut(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#define HM_GET_DECL(T) const T *hm_get_##T(const HashMap *hm, u64 hash);
HM_TYPES(HM_GET_DECL)
#undef HM_GET_DECL

const void **hm_get_ptr(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_HASHMAP_H__ */
