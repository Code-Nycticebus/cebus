#include "ht.h"
#include "clib/asserts.h"
#include "datatypes/integers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ht_free(HashTable *ht) {
  arena_free(&ht->arena);
  ht->nodes = NULL;
  ht->count = 0;
  ht->cap = 0;
}

static void ht_resize(HashTable *ht, usize size) {
  if (size < ht->cap) {
    return;
  }
  HashNode *nodes = ht->nodes;
  usize old_cap = ht->cap;
  ht->cap = size;
  ht->nodes = arena_temp_calloc(&ht->arena, sizeof(ht->nodes[0]) * size);
  for (usize i = 0; i < old_cap; i++) {
    if (nodes[i].occupied) {
      ht_insert(ht, nodes[i].key, nodes[i].value);
    }
  }
  arena_temp_free(nodes);
}

void ht_insert(HashTable *ht, u64 hash, HashValue value) {
  if (!(ht->count * 2 < ht->cap)) {
    ht_resize(ht, usize_max(100, ht->cap * 2));
  }
  usize idx = hash % ht->cap;
  if (!ht->nodes[idx].occupied || ht->nodes[idx].key == hash) {
    ht->nodes[idx] = (HashNode){.occupied = true, .key = hash, .value = value};
    ht->count++;
    return;
  }
  for (usize i = 0; i < ht->cap; i++) {
    idx = (idx + i * i) % ht->cap;
    if (!ht->nodes[idx].occupied || ht->nodes[idx].key == hash) {
      ht->nodes[idx] =
          (HashNode){.occupied = true, .key = hash, .value = value};
      ht->count++;
      return;
    }
  }
  clib_assert(false, "Unreachable: That means the table has overrun");
}

void ht_insert_bytes(HashTable *ht, u64 hash, Bytes bytes) {
  clib_assert((u64)bytes.data < (1L << 48), "pointer out of limit!");
  clib_assert(bytes.size < U16_MAX, "size out of limit!");
  ht_insert(
      ht, hash,
      (HashValue){.bytes.size = bytes.size, .bytes.data = (u64)bytes.data});
}

void ht_insert_copy(HashTable *ht, u64 hash, usize size, const void *value) {
  u8 *buffer = arena_alloc(&ht->arena, size);
  memcpy(buffer, value, size);
  ht_insert(ht, hash, (HashValue){.ptr = buffer});
}

bool ht_try_get(HashTable *ht, u64 hash, HashValue *value) {
  usize idx = hash % ht->cap;
  if (ht->nodes[idx].occupied && ht->nodes[idx].key == hash) {
    *value = ht->nodes[idx].value;
    return true;
  }
  for (usize i = 0; i < ht->cap; i++) {
    idx = (idx + i * i) % ht->cap;
    if (ht->nodes[idx].occupied && ht->nodes[idx].key == hash) {
      *value = ht->nodes[idx].value;
      return true;
    }
  }
  return false;
}

HashValue ht_get(HashTable *ht, u64 hash) {
  HashValue value = {0};
  clib_assert(ht_try_get(ht, hash, &value), "Value was not in hashtable: %lx",
              hash);
  return value;
}

bool ht_try_get_bytes(HashTable *ht, u64 hash, Bytes *bytes) {
  usize idx = hash % ht->cap;
  if (ht->nodes[idx].occupied && ht->nodes[idx].key == hash) {
    bytes->size = ht->nodes[idx].value.bytes.size;
    bytes->data = (void *)ht->nodes[idx].value.bytes.data;
    return true;
  }
  for (usize i = 0; i < ht->cap; i++) {
    idx = (idx + i * i) % ht->cap;
    if (ht->nodes[idx].occupied && ht->nodes[idx].key == hash) {
      bytes->size = ht->nodes[idx].value.bytes.size;
      bytes->data = (void *)ht->nodes[idx].value.bytes.data;
      return true;
    }
  }
  return false;
}

Bytes ht_get_bytes(HashTable *ht, u64 hash) {
  Bytes bytes = {0};
  clib_assert(ht_try_get_bytes(ht, hash, &bytes),
              "Value was not in hashtable: %lx", hash);
  return bytes;
}

bool ht_contains(HashTable *ht, u64 hash) {
  usize idx = hash % ht->cap;
  if (ht->nodes[idx].occupied && ht->nodes[idx].key == hash) {
    return true;
  }
  for (usize i = 0; i < ht->cap; i++) {
    idx = (idx + i * i) % ht->cap;
    if (ht->nodes[idx].occupied && ht->nodes[idx].key == hash) {
      return true;
    }
  }
  return false;
}
