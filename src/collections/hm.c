#include "hm.h"

#include "core/arena.h"
#include "core/asserts.h"
#include "core/defines.h"
#include "core/logging.h"
#include <stdio.h>
#include <string.h>

static void hm_resize(HashMap *hm) {
  usize old_cap = hm->cap;
  HashNode *old_nodes = hm->nodes;

  hm->cap *= 2;
  hm->nodes = arena_calloc_chunk(hm->arena, hm->cap * sizeof(hm->nodes[0]));
  hm->count = 0;
  for (usize i = 0; i < old_cap; ++i) {
    if (old_nodes[i].key) {
      hm_insert(hm, old_nodes[i].key, old_nodes[i].value);
    }
  }

  arena_free_chunk(hm->arena, old_nodes);
}

HashMap hm_create(Arena *arena, usize size) {
  HashMap hm = {0};
  hm.arena = arena;
  hm.cap = size;
  hm.nodes = arena_calloc_chunk(arena, hm.cap * sizeof(hm.nodes[0]));
  return hm;
}

bool hm_insert(HashMap *hm, u64 hash, HashValue value) {
  clib_assert(hash != 0, "Hash should not be zero: %" U64_HEX, hash);
  if (hm->cap <= hm->count) {
    hm_resize(hm);
  }

  while (true) {
    usize idx = hash % hm->cap;

    if (!hm->nodes[idx].key) {
      hm->nodes[idx] = (HashNode){.key = hash, .value = value};
      hm->count++;
      return true;
    }
    if (hm->nodes[idx].key == hash) {
      hm->nodes[idx].value = value;
      return false;
    }

    for (usize i = 0; i < hm->cap; i++) {
      idx = (idx + i * i) % hm->cap;
      if (!hm->nodes[idx].key) {
        hm->nodes[idx] = (HashNode){.key = hash, .value = value};
        hm->count++;
        return true;
      }
      if (hm->nodes[idx].key == hash) {
        hm->nodes[idx].value = value;
        return false;
      }
    }

    hm_resize(hm);
  }
}

HashValue *hm_get(const HashMap *hm, u64 hash) {
  usize idx = hash % hm->cap;
  if (hm->nodes[idx].key && hm->nodes[idx].key == hash) {
    return &hm->nodes[idx].value;
  }
  for (usize i = 0; i < hm->cap; i++) {
    idx = (idx + i * i) % hm->cap;
    if (hm->nodes[idx].key && hm->nodes[idx].key == hash) {
      return &hm->nodes[idx].value;
    }
  }
  for (usize i = 0; i < hm->cap; i++) {
    if (hm->nodes[i].key && hm->nodes[i].key == hash) {
      return &hm->nodes[idx].value;
    }
  }
  return NULL;
}
