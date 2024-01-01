#include "set.h"

#include "clib/asserts.h"
#include "datatypes/integers.h"

void set_free(Set *set) { arena_free(&set->arena); }

static void set_resize(Set *set, usize size) {
  if (size < set->cap) {
    return;
  }
  SetNode *nodes = set->nodes;
  usize old_cap = set->cap;
  set->cap = size;
  set->nodes = arena_temp_calloc(&set->arena, sizeof(set->nodes[0]) * size);
  for (usize i = 0; i < old_cap; i++) {
    if (nodes[i].occupied) {
      set_insert(set, nodes[i].hash);
    }
  }
  arena_temp_free(nodes);
}

void set_insert(Set *set, u64 hash) {
  if (!(set->count * 2 < set->cap)) {
    set_resize(set, usize_max(100, set->cap * 2));
  }
  usize idx = hash % set->cap;
  if (!set->nodes[idx].occupied || set->nodes[idx].hash == hash) {
    set->nodes[idx].occupied = true;
    set->nodes[idx].hash = hash;
    set->count++;
    return;
  }
  for (usize i = 0; i < set->cap; i++) {
    idx = (idx + i * i) % set->cap;
    if (!set->nodes[idx].occupied || set->nodes[idx].hash == hash) {
      set->nodes[idx].occupied = true;
      set->nodes[idx].hash = hash;
      set->count++;
      return;
    }
  }
  clib_assert(false, "Unreachable: That means the table has overrun");
}

void set_remove(Set *set, u64 hash) {
  usize idx = hash % set->cap;
  if (!set->nodes[idx].occupied || set->nodes[idx].hash == hash) {
    set->nodes[idx].occupied = false;
    set->count--;
    return;
  }
  for (usize i = 0; i < set->cap; i++) {
    idx = (idx + i * i) % set->cap;
    if (!set->nodes[idx].occupied || set->nodes[idx].hash == hash) {
      set->nodes[idx].occupied = false;
      set->count--;
      return;
    }
  }
  clib_assert(false, "Unreachable: That means the table has overrun");
}

bool set_contains(Set *set, u64 hash) {
  usize idx = hash % set->cap;
  if (set->nodes[idx].occupied && set->nodes[idx].hash == hash) {
    return true;
  }
  for (usize i = 0; i < set->cap; i++) {
    idx = (idx + i * i) % set->cap;
    if (set->nodes[idx].occupied && set->nodes[idx].hash == hash) {
      return true;
    }
  }
  return false;
}

bool set_is_subset(Set *subset, Set *off) {
  if (off->count <= subset->count) {
    return false;
  }
  for (usize i = 0; i < subset->cap; i++) {
    if (subset->nodes[i].occupied) {
      if (!set_contains(off, subset->nodes[i].hash)) {
        return false;
      }
    }
  }
  return true;
}

Set set_intersection(Set *set, Set *other, Arena *arena) {
  Set intersection = set_create(arena, usize_min(set->count, other->count));
  for (usize i = 0; i < set->cap; i++) {
    if (set->nodes[i].occupied) {
      if (set_contains(other, set->nodes[i].hash)) {
        set_add(&intersection, set->nodes[i].hash);
      }
    }
  }
  return intersection;
}

Set set_difference(Set *set, Set *other, Arena *arena) {
  Set difference = set_create(arena, usize_min(set->count, other->count));
  for (usize i = 0; i < set->cap; i++) {
    if (set->nodes[i].occupied) {
      if (!set_contains(other, set->nodes[i].hash)) {
        set_add(&difference, set->nodes[i].hash);
      }
    }
  }
  return difference;
}
