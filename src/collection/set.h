/* DOCUMENTATION
### Usage
Create a new Set with:
```c
Arena arena = {0};
Set set = set_create(&arena);
```

Then you can add elements by hash to the Set. ```set_add()``` returns false if
the Set already contains element.
```c
set_add(&set, str_hash(STR("Hello")));
set_add(&set, str_hash(STR("World")));
```

Test if an element is in the Set.
```c
set_contains(&set, str_hash(STR("Hello")));
set_contains(&set, str_hash(STR("World")));
```
*/

#ifndef __CLIB_SET_H__
#define __CLIB_SET_H__

#include "core/arena.h"
#include "core/defines.h"

//////////////////////////////////////////////////////////////////////////////

typedef struct {
  usize cap;
  usize count;
  Arena *arena;
  u64 *items;
} Set;

///////////////////////////////////////////////////////////////////////////////

Set set_create(Arena *arena);
Set set_with_size(Arena *arena, usize size);

Set set_copy(Arena *arena, Set *set);

void set_resize(Set *set, usize size);
void set_reserve(Set *set, usize size);

///////////////////////////////////////////////////////////////////////////////

bool set_add(Set *set, u64 hash);
void set_extend(Set *set, usize count, const u64 *hashes);
void set_update(Set *dest, const Set *set);

//////////////////////////////////////////////////////////////////////////////

bool set_contains(const Set *set, u64 hash);
bool set_eq(const Set *set, const Set *other);
bool set_subset(const Set *set, const Set *other);
bool set_disjoint(const Set *set, const Set *other);

///////////////////////////////////////////////////////////////////////////////

Set set_intersection(const Set *set, const Set *other, Arena *arena);
Set set_difference(const Set *set, const Set *other, Arena *arena);
Set set_union(const Set *set, const Set *other, Arena *arena);

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_SET_H__ */
