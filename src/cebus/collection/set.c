#include "set.h"

#include "cebus/type/integer.h"

#include <string.h>

//////////////////////////////////////////////////////////////////////////////

#define SET_DEFAULT_SIZE 8
#define SET_DELETED_HASH 0xdeaddeaddeaddead

//////////////////////////////////////////////////////////////////////////////

Set set_create(Arena *arena) {
  Set set = {0};
  set.arena = arena;
  return set;
}

Set set_with_size(Arena *arena, usize size) {
  Set set = {0};
  set.arena = arena;
  set.cap = usize_max(size, SET_DEFAULT_SIZE);
  set.items = arena_calloc_chunk(arena, set.cap * sizeof(set.items[0]));
  return set;
}

void set_clear(Set *set) {
  set->count = 0;
  set->deleted = 0;
  memset(set->items, 0, set->cap * sizeof(set->items[0]));
}

Set set_copy(Arena *arena, Set *set) {
  Set new = set_with_size(arena, set->count * 2);
  for (size_t i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      set_add(&new, set->items[i]);
    }
  }
  return new;
}

void set_resize(Set *set, usize size) {
  if (size < set->cap) {
    return;
  }
  usize old_cap = set->cap;
  u64 *old_items = set->items;

  set->cap = size == 0 ? SET_DEFAULT_SIZE : size;
  set->items = arena_calloc_chunk(set->arena, set->cap * sizeof(set->items[0]));

  set->count = 0;
  set->deleted = 0;
  for (usize i = 0; i < old_cap; ++i) {
    if (old_items[i] && old_items[i] != SET_DELETED_HASH) {
      set_add(set, old_items[i]);
    }
  }
  arena_free_chunk(set->arena, old_items);
}

void set_reserve(Set *set, usize size) {
  const usize required_size = set->count + size;
  if (required_size < set->cap) {
    return;
  }
  usize new_size = set->cap == 0 ? SET_DEFAULT_SIZE : set->cap;
  while (new_size < required_size) {
    new_size *= 2;
  }
  set_resize(set, new_size);
}

//////////////////////////////////////////////////////////////////////////////

bool set_add(Set *set, u64 hash) {
  if (hash == 0 || hash == SET_DELETED_HASH) {
    hash = u64_hash(hash);
  }
  if (set->cap <= set->count + set->deleted) {
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

void set_extend(Set *set, usize count, const u64 *hashes) {
  set_reserve(set, count);
  for (usize i = 0; i < count; i++) {
    set_add(set, hashes[i]);
  }
}

void set_update(Set *dest, const Set *set) {
  set_reserve(dest, set->count);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i]) {
      set_add(dest, set->items[i]);
    }
  }
}

bool set_remove(Set *set, u64 hash) {
  if (set->count == 0) {
    return false;
  }
  if (hash == 0 || hash == SET_DELETED_HASH) {
    hash = u64_hash(hash);
  }

  usize idx = hash % set->cap;
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[idx] && set->items[idx] == hash) {
      set->items[idx] = SET_DELETED_HASH;
      set->count--;
      set->deleted++;
      return true;
    }
    idx = (idx + i * i) % set->cap;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////////////

bool set_contains(const Set *set, u64 hash) {
  if (hash == 0 || hash == SET_DELETED_HASH) {
    hash = u64_hash(hash);
  }

  usize idx = hash % set->cap;
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[idx] == 0) {
      return false;
    }
    if (set->items[idx] == hash) {
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
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(other, set->items[i])) {
        return false;
      }
    }
  }
  return true;
}

bool set_subset(const Set *set, const Set *other) {
  if (other->count < set->count) {
    return false;
  }
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(other, set->items[i])) {
        return false;
      }
    }
  }
  return true;
}

bool set_disjoint(const Set *set, const Set *other) {
  if (other->count == 0 || set->count == 0) {
    return true;
  }

  if (other->cap < set->cap) {
    const Set *temp = set;
    set = other;
    other = temp;
  }
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (set_contains(other, set->items[i])) {
        return false;
      }
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////

Set set_intersection(const Set *set, const Set *other, Arena *arena) {
  if (other->cap < set->cap) {
    const Set *temp = set;
    set = other;
    other = temp;
  }

  Set intersection = set_create(arena);
  set_reserve(&intersection, usize_min(set->count, other->count) * 2);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (set_contains(other, set->items[i])) {
        set_add(&intersection, set->items[i]);
      }
    }
  }
  return intersection;
}

Set set_difference(const Set *set, const Set *other, Arena *arena) {
  Set difference = set_create(arena);
  set_reserve(&difference, set->count * 2);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(other, set->items[i])) {
        set_add(&difference, set->items[i]);
      }
    }
  }
  return difference;
}

Set set_union(const Set *set, const Set *other, Arena *arena) {
  Set _union = set_with_size(arena, set->count * 2);
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(other, set->items[i])) {
        set_add(&_union, set->items[i]);
      }
    }
  }

  for (usize i = 0; i < other->cap; i++) {
    if (other->items[i] && set->items[i] != SET_DELETED_HASH) {
      if (!set_contains(set, other->items[i])) {
        set_add(&_union, other->items[i]);
      }
    }
  }
  return _union;
}

//////////////////////////////////////////////////////////////////////////////
