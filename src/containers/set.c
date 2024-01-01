#include "set.h"

#include "datatypes/integers.h"

Set set_create(Arena *arena, usize size) {
  Set set = {0};
  set.cap = size;
  set.items = arena_calloc(arena, sizeof(set.items[0]) * size);
  return set;
}

bool set_add(Set *set, u64 hash) {
  usize idx = hash % set->cap;
  if (!set->items[idx].occupied || set->items[idx].hash == hash) {
    set->items[idx].occupied = true;
    set->items[idx].hash = hash;
    set->count++;
    return true;
  }
  for (usize i = 0; i < set->cap; i++) {
    idx = (idx + i * i) % set->cap;
    if (!set->items[idx].occupied || set->items[idx].hash == hash) {
      set->items[idx].occupied = true;
      set->items[idx].hash = hash;
      set->count++;
      return true;
    }
  }
  return false;
}

bool set_remove(Set *set, u64 hash) {
  usize idx = hash % set->cap;
  if (!set->items[idx].occupied || set->items[idx].hash == hash) {
    set->items[idx].occupied = false;
    set->count--;
    return true;
  }
  for (usize i = 0; i < set->cap; i++) {
    idx = (idx + i * i) % set->cap;
    if (!set->items[idx].occupied || set->items[idx].hash == hash) {
      set->items[idx].occupied = false;
      set->count--;
      return true;
    }
  }
  return false;
}

bool set_contains(Set *set, u64 hash) {
  usize idx = hash % set->cap;
  if (set->items[idx].occupied && set->items[idx].hash == hash) {
    return true;
  }
  for (usize i = 0; i < set->cap; i++) {
    idx = (idx + i * i) % set->cap;
    if (set->items[idx].occupied && set->items[idx].hash == hash) {
      return true;
    }
  }
  return false;
}

bool set_subset(Set *set, Set *other) {
  if (other->count <= set->count) {
    return false;
  }
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i].occupied) {
      if (!set_contains(other, set->items[i].hash)) {
        return false;
      }
    }
  }
  return true;
}

Set set_intersection(Set *set, Set *other, Arena *arena) {
  Set intersection = set_create(arena, usize_min(set->count, other->count));
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i].occupied) {
      if (set_contains(other, set->items[i].hash)) {
        set_add(&intersection, set->items[i].hash);
      }
    }
  }
  return intersection;
}

Set set_difference(Set *set, Set *other, Arena *arena) {
  Set difference = set_create(arena, usize_min(set->count, other->count));
  for (usize i = 0; i < set->cap; i++) {
    if (set->items[i].occupied) {
      if (!set_contains(other, set->items[i].hash)) {
        set_add(&difference, set->items[i].hash);
      }
    }
  }
  return difference;
}
