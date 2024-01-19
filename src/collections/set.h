#ifndef __CLIB_SET_H__
#define __CLIB_SET_H__

#include "core/arena.h"
#include "core/defines.h"

typedef struct {
  usize cap;
  usize count;
  Arena *arena;
  u64 *items;
} Set;

Set set_create(Arena *arena, usize size);
Set set_copy(Arena *arena, Set *set);

void set_reserve(Set *set, usize size);

bool set_add(Set *set, u64 hash);
void set_extend(Set *set, usize count, u64 hashes[count]);

bool set_contains(const Set *set, u64 hash);
bool set_eq(const Set *set, const Set *other);
bool set_subset(const Set *set, const Set *other);

Set set_intersection(const Set *set, const Set *other, Arena *arena);
Set set_difference(const Set *set, const Set *other, Arena *arena);

#endif // !__CLIB_SET_H__
