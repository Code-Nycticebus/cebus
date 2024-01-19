#include "set.h"

#include "core/asserts.h"
#include "types/integers.h"
#include <string.h>

static void set_resize(Set *set, usize new_size) {
  usize old_cap = set->cap;
  u64 *old_items = set->items;

  set->cap = usize_max(new_size, 10);
  set->items = arena_calloc_chunk(set->arena, set->cap * sizeof(set->items[0]));

  if (old_items) {
    set->count = 0;
    for (usize i = 0; i < old_cap; ++i) {
      if (old_items[i]) {
        set_add(set, old_items[i]);
      }
    }
    arena_free_chunk(set->arena, old_items);
  }
}

Set set_create(Arena *arena) {
  Set set = {0};
  set.arena = arena;
  return set;
}

Set set_copy(Arena *arena, Set *set) {
  Set new_set = {0};
  new_set.count = set->count;
  new_set.cap = set->cap;
  set->arena = arena;
  new_set.items = arena_alloc_chunk(arena, set->cap * sizeof(set->items[0]));
  memcpy(new_set.items, set->items, set->cap * sizeof(set->items[0]));
  return new_set;
}

void set_reserve(Set *set, usize size) {
  if (size < set->cap) {
    return;
  }
  set_resize(set, size);
}

bool set_add(Set *set, u64 hash) {
  clib_assert(hash != 0, "Hash should not be zero: %" U64_HEX, hash);
  if (set->cap <= set->count) {
    set_resize(set, set->cap * 2);
  }

  while (true) {
    usize idx = hash % set->cap;

    for (usize i = 0; i < set->cap; i++) {
      if (!set->items[idx]) {
        set->items[idx] = hash;
        set->count++;
        return true;
      }
      if (set->items[idx] == hash) {
        return false;
      }
      idx = (idx + i * i) % set->cap;
    }

    set_resize(set, set->cap * 2);
  }
}

void set_extend(Set *set, usize count, u64 hashes[count]) {
  set_reserve(set, count);
  for (usize i = 0; i < count; i++) {
    set_add(set, hashes[i]);
  }
}

bool set_contains(const Set *set, u64 hash) {
  usize idx = hash % set->cap;
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[idx] && set->items[idx] == hash) {
      return true;
    }
    idx = (idx + i * i) % set->cap;
  }
  return false;
}

bool set_eq(const Set *set, const Set *other) {
  if (other->count != set->count) {
    return false;
  }

  if (other->cap < set->cap) {
    const Set *temp = set;
    set = other;
    other = temp;
  }
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i]) {
      if (!set_contains(other, set->items[i])) {
        return false;
      }
    }
  }
  return true;
}

bool set_subset(const Set *set, const Set *other) {
  if (other->count <= set->count) {
    return false;
  }
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i]) {
      if (!set_contains(other, set->items[i])) {
        return false;
      }
    }
  }
  return true;
}

Set set_intersection(const Set *set, const Set *other, Arena *arena) {
  if (other->cap < set->cap) {
    const Set *temp = set;
    set = other;
    other = temp;
  }

  Set intersection = set_create(arena);
  set_reserve(&intersection, usize_min(set->count, other->count) * 2);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i]) {
      if (set_contains(other, set->items[i])) {
        set_add(&intersection, set->items[i]);
      }
    }
  }
  return intersection;
}

Set set_difference(const Set *set, const Set *other, Arena *arena) {
  if (other->cap < set->cap) {
    const Set *temp = set;
    set = other;
    other = temp;
  }
  Set difference = set_create(arena);
  set_reserve(&difference, usize_min(set->count, other->count) * 2);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i]) {
      if (!set_contains(other, set->items[i])) {
        set_add(&difference, set->items[i]);
      }
    }
  }
  return difference;
}
