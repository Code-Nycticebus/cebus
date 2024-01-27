/* DOCUMENTATION
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

#ifndef __CLIB_HT_H__
#define __CLIB_HT_H__

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
  Arena *arena;
  HashNode *nodes;
} HashMap;

///////////////////////////////////////////////////////////////////////////////

HashMap hm_create(Arena *arena);
HashMap hm_with_size(Arena *arena, usize size);

HashMap hm_copy(HashMap *hm, Arena *arena);

void hm_resize(HashMap *hm, usize size);
void hm_reserve(HashMap *hm, usize size);

///////////////////////////////////////////////////////////////////////////////

bool hm_insert(HashMap *hm, u64 hash, HashValue value);
void hm_update(HashMap *hm, HashMap *other);
// FIXME it is dangerous to return a pointer here
HashValue *hm_get(const HashMap *hm, u64 hash);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_HT_H__ */
