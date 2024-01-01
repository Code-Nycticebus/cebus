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
