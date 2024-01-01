#include "set.h"

#include "datatypes/integers.h"

Set set_create(Arena *arena, usize size) {
  Set set = {0};
  set.cap = size;
  set.nodes = arena_calloc(arena, sizeof(set.nodes[0]) * size);
  return set;
}

bool set_add(Set *set, u64 hash) {
  usize idx = hash % set->cap;
  if (!set->nodes[idx].occupied || set->nodes[idx].hash == hash) {
    set->nodes[idx].occupied = true;
    set->nodes[idx].hash = hash;
    set->count++;
    return true;
  }
  for (usize i = 0; i < set->cap; i++) {
    idx = (idx + i * i) % set->cap;
    if (!set->nodes[idx].occupied || set->nodes[idx].hash == hash) {
      set->nodes[idx].occupied = true;
      set->nodes[idx].hash = hash;
      set->count++;
      return true;
    }
  }
  return false;
}

bool set_remove(Set *set, u64 hash) {
  usize idx = hash % set->cap;
  if (!set->nodes[idx].occupied || set->nodes[idx].hash == hash) {
    set->nodes[idx].occupied = false;
    set->count--;
    return true;
  }
  for (usize i = 0; i < set->cap; i++) {
    idx = (idx + i * i) % set->cap;
    if (!set->nodes[idx].occupied || set->nodes[idx].hash == hash) {
      set->nodes[idx].occupied = false;
      set->count--;
      return true;
    }
  }
  return false;
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
