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

typedef enum {
  HM_NONE,
  HM_F32,
  HM_F64,
  HM_I32,
  HM_U32,
  HM_I64,
  HM_U64,
  HM_PTR,
} HashTypes;

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
  HashTypes type;
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

bool hm_insert_f32(HashMap *hm, u64 hash, f32 value);
bool hm_insert_f64(HashMap *hm, u64 hash, f64 value);
bool hm_insert_i32(HashMap *hm, u64 hash, i32 value);
bool hm_insert_u32(HashMap *hm, u64 hash, u32 value);
bool hm_insert_i64(HashMap *hm, u64 hash, i64 value);
bool hm_insert_u64(HashMap *hm, u64 hash, u64 value);
bool hm_insert_ptr(HashMap *hm, u64 hash, void *value);

void hm_update(HashMap *hm, HashMap *other);

bool hm_remove(HashMap *hm, usize hash);

const HashValue *hm_get(const HashMap *hm, u64 hash);
HashValue *hm_get_mut(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_HASHMAP_H__ */
