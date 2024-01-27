#include "hm.h"

#include "core/arena.h"
#include "core/defines.h"
#include "type/integer.h"

#include <stdio.h>
#include <string.h>

#define HM_DEFAULT_SIZE 8

////////////////////////////////////////////////////////////////////////////

HashMap hm_create(Arena *arena) {
  HashMap hm = {0};
  hm.arena = arena;
  return hm;
}

HashMap hm_with_size(Arena *arena, usize size) {
  HashMap hm = {0};
  hm.arena = arena;
  hm.cap = usize_max(size, HM_DEFAULT_SIZE);
  hm.nodes = arena_calloc_chunk(arena, hm.cap * sizeof(hm.nodes[0]));
  return hm;
}

void hm_clear(HashMap *hm) {
  hm->count = 0;
  memset(hm->nodes, 0, hm->cap * sizeof(hm->nodes[0]));
}

HashMap hm_copy(HashMap *hm, Arena *arena) {
  HashMap new = hm_with_size(arena, hm->count * 2);
  for (size_t i = 0; i < hm->cap; i++) {
    hm_insert(&new, hm->nodes[i].key, hm->nodes[i].value);
  }
  return new;
}

void hm_resize(HashMap *hm, usize size) {
  if (size < hm->cap) {
    return;
  }
  usize old_cap = hm->cap;
  HashNode *old_nodes = hm->nodes;

  hm->cap = size == 0 ? HM_DEFAULT_SIZE : size;
  hm->nodes = arena_calloc_chunk(hm->arena, hm->cap * sizeof(hm->nodes[0]));

  hm->count = 0;
  for (usize i = 0; i < old_cap; ++i) {
    if (old_nodes[i].key) {
      hm_insert(hm, old_nodes[i].key, old_nodes[i].value);
    }
  }
  arena_free_chunk(hm->arena, old_nodes);
}

void hm_reserve(HashMap *hm, usize size) {
  const usize required_size = hm->count + size;
  if (required_size < hm->cap) {
    return;
  }
  usize new_size = hm->cap == 0 ? HM_DEFAULT_SIZE : hm->cap;
  while (new_size < required_size) {
    new_size *= 2;
  }
  hm_resize(hm, new_size);
}

////////////////////////////////////////////////////////////////////////////

bool hm_insert(HashMap *hm, u64 hash, HashValue value) {
  if (hash == 0) {
    hash = u64_hash(hash);
  }
  if (hm->cap <= hm->count) {
    hm_resize(hm, hm->cap * 2);
  }

  while (true) {
    usize idx = hash % hm->cap;

    for (usize i = 0; i < hm->cap; i++) {
      if (!hm->nodes[idx].key) {
        hm->nodes[idx] = (HashNode){.key = hash, .value = value};
        hm->count++;
        return true;
      }
      if (hm->nodes[idx].key == hash) {
        hm->nodes[idx].value = value;
        return false;
      }
      idx = (idx + i * i) % hm->cap;
    }

    hm_resize(hm, hm->cap * 2);
  }
}

void hm_update(HashMap *hm, HashMap *other) {
  hm_reserve(hm, other->count);
  for (usize i = 0; i < other->cap; ++i) {
    if (other->nodes[i].key) {
      hm_insert(hm, other->nodes[i].key, other->nodes[i].value);
    }
  }
}

HashValue *hm_get(const HashMap *hm, u64 hash) {
  if (hm->count == 0) {
    return NULL;
  }
  if (hash == 0) {
    hash = u64_hash(hash);
  }

  usize idx = hash % hm->cap;
  for (usize i = 0; i < hm->cap; i++) {
    if (hm->nodes[idx].key && hm->nodes[idx].key == hash) {
      return &hm->nodes[idx].value;
    }
    idx = (idx + i * i) % hm->cap;
  }
  return NULL;
}

////////////////////////////////////////////////////////////////////////////
