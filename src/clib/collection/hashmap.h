/* DOCUMENTATION
### Overview
This `HashMap` implementation stores only the keys hash and a `HashValue`, as
retrieving key values directly from the `HashMap` is often not needed. To store
the keys, you should be using a seperate array.
### Usage
Create a new HashMap with:
```c
Arena arena = {0};
HashMap* hm = hm_create(&arena);
hm_reserve(hm, 10); // optional
```

Then you can add elements by hash to the HashMap.
```c
hm_insert_i32(hm, str_hash(STR("Hello")), 69);
hm_insert_i32(hm, str_hash(STR("World")), 420);
```

Now you can get pointers to values by passing in the hash of the element.
> :warning: Don't safe pointers that point inside the HashMap!
```c
hm_get_i32(hm, str_hash(STR("Hello")));
hm_get_i32(hm, str_hash(STR("World")));
```
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
bool hm_insert_i32(HashMap *hm, u64 hash, i32 value);
bool hm_insert_u32(HashMap *hm, u64 hash, u32 value);
bool hm_insert_i64(HashMap *hm, u64 hash, i64 value);
bool hm_insert_u64(HashMap *hm, u64 hash, u64 value);
bool hm_insert_mut_ptr(HashMap *hm, u64 hash, void *value);
bool hm_insert_ptr(HashMap *hm, u64 hash, const void *value);

///////////////////////////////////////////////////////////////////////////////

f32 *hm_get_f32_mut(const HashMap *hm, u64 hash);
f64 *hm_get_f64_mut(const HashMap *hm, u64 hash);
i32 *hm_get_i32_mut(const HashMap *hm, u64 hash);
u32 *hm_get_u32_mut(const HashMap *hm, u64 hash);
i64 *hm_get_i64_mut(const HashMap *hm, u64 hash);
u64 *hm_get_u64_mut(const HashMap *hm, u64 hash);
void **hm_get_ptr_mut(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

const f32 *hm_get_f32(const HashMap *hm, u64 hash);
const f64 *hm_get_f64(const HashMap *hm, u64 hash);
const i32 *hm_get_i32(const HashMap *hm, u64 hash);
const u32 *hm_get_u32(const HashMap *hm, u64 hash);
const i64 *hm_get_i64(const HashMap *hm, u64 hash);
const u64 *hm_get_u64(const HashMap *hm, u64 hash);
const void **hm_get_ptr(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_HASHMAP_H__ */
