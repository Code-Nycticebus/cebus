#ifndef __CLIB_SET_H__
#define __CLIB_SET_H__

#include "clib/arena.h"
#include "datatypes/datatypes.h"

typedef struct {
  u64 hash;
  bool occupied;
} SetNode;

typedef struct {
  usize cap;
  usize count;
  SetNode *nodes;
} Set;

Set set_create(Arena *arena, usize size);

bool set_add(Set *set, u64 hash);
bool set_remove(Set *set, u64 hash);

bool set_contains(Set *set, u64 hash);
bool set_is_subset(Set *subset, Set *off);

Set set_intersection(Set *set, Set *other, Arena *arena);
Set set_difference(Set *set, Set *other, Arena *arena);

#endif // !__CLIB_SET_H__
