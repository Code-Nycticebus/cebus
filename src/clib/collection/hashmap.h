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

- `hm_insert_<T>`: Insert `u8`, `i8`, `u32`, `i32`, `usize`, `f32` or `f64`
values.
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

bool hm_insert_f32(HashMap *hm, u64 hash, f32 value);
bool hm_insert_f64(HashMap *hm, u64 hash, f64 value);
bool hm_insert_i8(HashMap *hm, u64 hash, i8 value);
bool hm_insert_u8(HashMap *hm, u64 hash, u8 value);
bool hm_insert_i16(HashMap *hm, u64 hash, i16 value);
bool hm_insert_u16(HashMap *hm, u64 hash, u16 value);
bool hm_insert_i32(HashMap *hm, u64 hash, i32 value);
bool hm_insert_u32(HashMap *hm, u64 hash, u32 value);
bool hm_insert_i64(HashMap *hm, u64 hash, i64 value);
bool hm_insert_u64(HashMap *hm, u64 hash, u64 value);
bool hm_insert_usize(HashMap *hm, u64 hash, usize value);
bool hm_insert_mut_ptr(HashMap *hm, u64 hash, void *value);
bool hm_insert_ptr(HashMap *hm, u64 hash, const void *value);

///////////////////////////////////////////////////////////////////////////////

f32 *hm_get_f32_mut(const HashMap *hm, u64 hash);
f64 *hm_get_f64_mut(const HashMap *hm, u64 hash);
i8 *hm_get_i8_mut(const HashMap *hm, u64 hash);
u8 *hm_get_u8_mut(const HashMap *hm, u64 hash);
i16 *hm_get_i16_mut(const HashMap *hm, u64 hash);
u16 *hm_get_u16_mut(const HashMap *hm, u64 hash);
i32 *hm_get_i32_mut(const HashMap *hm, u64 hash);
u32 *hm_get_u32_mut(const HashMap *hm, u64 hash);
i64 *hm_get_i64_mut(const HashMap *hm, u64 hash);
u64 *hm_get_u64_mut(const HashMap *hm, u64 hash);
usize *hm_get_usize_mut(const HashMap *hm, u64 hash);
void *hm_get_ptr_mut(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

const f32 *hm_get_f32(const HashMap *hm, u64 hash);
const f64 *hm_get_f64(const HashMap *hm, u64 hash);
const i8 *hm_get_i8(const HashMap *hm, u64 hash);
const u8 *hm_get_u8(const HashMap *hm, u64 hash);
const i16 *hm_get_i16(const HashMap *hm, u64 hash);
const u16 *hm_get_u16(const HashMap *hm, u64 hash);
const i32 *hm_get_i32(const HashMap *hm, u64 hash);
const u32 *hm_get_u32(const HashMap *hm, u64 hash);
const i64 *hm_get_i64(const HashMap *hm, u64 hash);
const u64 *hm_get_u64(const HashMap *hm, u64 hash);
const usize *hm_get_usize(const HashMap *hm, u64 hash);
const void *hm_get_ptr(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_HASHMAP_H__ */
