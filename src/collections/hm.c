#include "hm.h"

#include "core/asserts.h"
#include <stdio.h>

HashMap hm_create(Arena *arena, usize size) {
  HashMap hm = {0};
  hm.cap = size;
  hm.nodes = arena_calloc(arena, size * sizeof(hm.nodes[0]));
  return hm;
}

void hm_insert(HashMap *hm, u64 hash, HashValue value) {
  clib_assert(hash != 0, "Hash should not be zero: %" U64_HEX, hash);
  clib_assert(hm->count < hm->cap, "Table full!");
  usize idx = hash % hm->cap;
  if (!hm->nodes[idx].key || hm->nodes[idx].key == hash) {
    hm->nodes[idx] = (HashNode){.key = hash, .value = value};
    hm->count++;
    return;
  }

  for (usize i = 0; i < hm->cap; i++) {
    idx = (idx + i * i) % hm->cap;
    if (!hm->nodes[idx].key || hm->nodes[idx].key == hash) {
      hm->nodes[idx] = (HashNode){.key = hash, .value = value};
      hm->count++;
      return;
    }
  }

  for (usize i = 0; i < hm->cap; i++) {
    if (!hm->nodes[i].key || hm->nodes[i].key == hash) {
      hm->nodes[i] = (HashNode){.key = hash, .value = value};
      hm->count++;
      return;
    }
  }
  clib_assert(false, "Unreachable: table overrun: %" USIZE_FMT " elements",
              hm->cap);
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
