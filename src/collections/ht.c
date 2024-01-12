#include "ht.h"

#include "core/asserts.h"
#include <stdio.h>

HashTable ht_create(Arena *arena, usize size) {
  HashTable ht = {0};
  ht.cap = size;
  ht.nodes = arena_calloc(arena, size * sizeof(ht.nodes[0]));
  return ht;
}

void ht_insert(HashTable *ht, u64 hash, HashValue value) {
  clib_assert(hash != 0, "Hash should not be zero: %" U64_HEX, hash);
  clib_assert(ht->count < ht->cap, "Table full!");
  usize idx = hash % ht->cap;
  if (!ht->nodes[idx].key || ht->nodes[idx].key == hash) {
    ht->nodes[idx] = (HashNode){.key = hash, .value = value};
    ht->count++;
    return;
  }

  for (usize i = 0; i < ht->cap; i++) {
    idx = (idx + i * i) % ht->cap;
    if (!ht->nodes[idx].key || ht->nodes[idx].key == hash) {
      ht->nodes[idx] = (HashNode){.key = hash, .value = value};
      ht->count++;
      return;
    }
  }

  for (usize i = 0; i < ht->cap; i++) {
    if (!ht->nodes[i].key || ht->nodes[i].key == hash) {
      ht->nodes[i] = (HashNode){.key = hash, .value = value};
      ht->count++;
      return;
    }
  }
  clib_assert(false, "Unreachable: table overrun: %" USIZE_FMT " elements",
              ht->cap);
}

HashValue *ht_get(const HashTable *ht, u64 hash) {
  usize idx = hash % ht->cap;
  if (ht->nodes[idx].key && ht->nodes[idx].key == hash) {
    return &ht->nodes[idx].value;
  }
  for (usize i = 0; i < ht->cap; i++) {
    idx = (idx + i * i) % ht->cap;
    if (ht->nodes[idx].key && ht->nodes[idx].key == hash) {
      return &ht->nodes[idx].value;
    }
  }
  for (usize i = 0; i < ht->cap; i++) {
    if (ht->nodes[i].key && ht->nodes[i].key == hash) {
      return &ht->nodes[idx].value;
    }
  }
  return NULL;
}
