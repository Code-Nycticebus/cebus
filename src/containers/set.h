#ifndef __CLIB_SET_H__
#define __CLIB_SET_H__

#include "clib/arena.h"
#include "datatypes/datatypes.h"

typedef struct {
  u64 hash;
  bool occupied;
} SetNode;

typedef struct {
  Arena arena;
  usize cap;
  usize count;
  SetNode *nodes;
} Set;

void set_free(Set *set);

void set_insert(Set *set, u64 hash);
void set_remove(Set *set, u64 hash);
bool set_contains(Set *set, u64 hash);
bool set_is_subset(Set *subset, Set *off);

Set set_intersection(Set *set, Set *other, Arena *arena);
Set set_difference(Set *set, Set *other, Arena *arena);

#endif // !__CLIB_SET_H__
