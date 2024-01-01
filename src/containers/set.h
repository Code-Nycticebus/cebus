#ifndef __CLIB_SET_H__
#define __CLIB_SET_H__

#include "clib/arena.h"
#include "datatypes/datatypes.h"

typedef struct {
  usize cap;
  usize count;
  struct {
    u64 hash;
    bool occupied;
  } *items;
} Set;

Set set_create(Arena *arena, usize size);

bool set_add(Set *set, u64 hash);
bool set_remove(Set *set, u64 hash);

bool set_contains(Set *set, u64 hash);
bool set_eq(Set *set, Set *other);
bool set_subset(Set *set, Set *other);

Set set_intersection(Set *set, Set *other, Arena *arena);
Set set_difference(Set *set, Set *other, Arena *arena);

#endif // !__CLIB_SET_H__
