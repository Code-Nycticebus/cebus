#include "hm.h"

#include "core/arena.h"
#include "core/asserts.h"
#include "core/defines.h"
#include "core/logging.h"
#include "types/integers.h"

#include <stdio.h>
#include <string.h>

static void hm_resize(HashMap *hm, usize size) {
  usize old_cap = hm->cap;
  HashNode *old_nodes = hm->nodes;

  hm->cap = usize_max(size, 10);
  hm->nodes = arena_calloc_chunk(hm->arena, hm->cap * sizeof(hm->nodes[0]));

  if (old_nodes) {
    hm->count = 0;
    for (usize i = 0; i < old_cap; ++i) {
      if (old_nodes[i].key) {
        hm_insert(hm, old_nodes[i].key, old_nodes[i].value);
      }
    }
    arena_free_chunk(hm->arena, old_nodes);
  }
}

HashMap hm_create(Arena *arena) {
  HashMap hm = {0};
  hm.arena = arena;
  return hm;
}

void hm_reserve(HashMap *hm, usize size) {
  if (size < hm->cap) {
    return;
  }
  hm_resize(hm, size);
}

bool hm_insert(HashMap *hm, u64 hash, HashValue value) {
  clib_assert(hash != 0, "Hash should not be zero: %" U64_HEX, hash);
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

HashValue *hm_get(const HashMap *hm, u64 hash) {
  if (hm->count == 0) {
    return NULL;
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
