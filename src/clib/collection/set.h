/* DOCUMENTATION
My `Set` implementation follows the same principle as my `HashMap`: it stores
only the hashes for lookup. This means you get efficient way to check
if something is in the set without the overhead of storing the actual elements.
It’s pretty handy for cases where you just need to keep track of existence
rather than the elements themselves.

## Initialization

To start using the set, you need to initialize an `Arena` and create a new set:

```c
Arena arena = {0};
Set set = set_create(&arena);
```

## Set Operations

Perform basic set operations such as adding, removing, and extending sets with
multiple elements:

- `set_add`: Add an element to the set. Returns false if the element already
exists.
- `set_remove`: Remove an element from the set.
- `set_extend`: Add multiple elements to the set at once.

Copy and resize sets for handling dynamic set sizes:

- `set_copy`: Create a duplicate of a set.
- `set_resize`: Adjust the capacity of the set. Used for preallocating
space.
- `set_reserve`: Reserves space in the set. Used before adding multiple
elements


## Set Query Operations

Query the set for the presence of elements, equality with another set, or subset
and disjoint relationships:

- `set_contains`: Check if an element is in the set.
- `set_eq`: Check if two sets are equal.
- `set_subset`: Determine if one set is a subset of another.
- `set_disjoint`: Determine if two sets have no elements in common.

## Set Algebraic Operations

Combine sets or find their differences using algebraic set operations:

- `set_intersection`: Find the intersection of two sets.
- `set_difference`: Find the difference between two sets.
- `set_union`: Combine two sets into a union.
*/

#ifndef __CLIB_SET_H__
#define __CLIB_SET_H__

#include "clib/core/arena.h"
#include "clib/core/defines.h"

//////////////////////////////////////////////////////////////////////////////

typedef struct {
  usize cap;
  usize count;
  usize deleted;
  Arena *arena;
  u64 *items;
} Set;

///////////////////////////////////////////////////////////////////////////////

Set set_create(Arena *arena);
Set set_with_size(Arena *arena, usize size);

void set_clear(Set *set);

Set set_copy(Arena *arena, Set *set);

void set_resize(Set *set, usize size);
void set_reserve(Set *set, usize size);

///////////////////////////////////////////////////////////////////////////////

bool set_add(Set *set, u64 hash);
void set_extend(Set *set, usize count, const u64 *hashes);
void set_update(Set *dest, const Set *set);

bool set_remove(Set *set, u64 hash);

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
