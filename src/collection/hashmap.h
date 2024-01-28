/* DOCUMENTATION
### Overview
This `HashMap` implementation stores only the keys hash and a `HashValue`, as
retrieving key values directly from the `HashMap` is often not needed. To store
the keys, you should be using a seperate array.
### Usage
Create a new HashMap with:
```c
Arena arena = {0};
HashMap hm = hm_create(&arena);
hm_reserve(&hm, 10); // optional
```

Then you can add elements by hash to the HashMap.
```c
hm_insert(&set, str_hash(STR("Hello")), HashValue(i64, 69));
hm_insert(&set, str_hash(STR("World")), HashValue(i64, 420));
```

Now you can get the values by passing in the hash of the element.
```c
hm_get(&set, str_hash(STR("Hello")))->as.i64;
hm_get(&set, str_hash(STR("World")))->as.i64;
```
*/

#ifndef __CLIB_HASHMAP_H__
#define __CLIB_HASHMAP_H__

#include "core/arena.h"
#include "core/defines.h"

////////////////////////////////////////////////////////////////////////////

typedef struct {
  union {
    f32 f32;
    f64 f64;
    i32 i32;
    u32 u32;
    i64 i64;
    u64 u64;
    void *ptr;
  } as;
} HashValue;

#define HashValue(T, value)                                                    \
  (HashValue) { .as.T = value }

typedef struct {
  u64 key;
  HashValue value;
} HashNode;

typedef struct {
  usize cap;
  usize count;
  usize deleted;
  Arena *arena;
  HashNode *nodes;
} HashMap;

///////////////////////////////////////////////////////////////////////////////

HashMap hm_create(Arena *arena);
HashMap hm_with_size(Arena *arena, usize size);

void hm_clear(HashMap *hm);

HashMap hm_copy(HashMap *hm, Arena *arena);

void hm_resize(HashMap *hm, usize size);
void hm_reserve(HashMap *hm, usize size);

///////////////////////////////////////////////////////////////////////////////

bool hm_insert(HashMap *hm, u64 hash, HashValue value);
void hm_update(HashMap *hm, HashMap *other);

bool hm_remove(HashMap *hm, usize hash);

const HashValue *hm_get(const HashMap *hm, u64 hash);
HashValue *hm_get_mut(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_HASHMAP_H__ */
