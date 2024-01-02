#include "ht.h"

#include <string.h>

#include "clib/asserts.h"

HashTable ht_create(Arena *arena, usize size) {
  HashTable ht = {0};
  ht.arena = arena;
  ht.cap = size;
  ht.nodes = arena_calloc(arena, size * sizeof(ht.nodes[0]));
  return ht;
}

void ht_insert(HashTable *ht, u64 hash, HashValue value) {
  clib_assert(hash, "Invalid hash: 0x%lx", hash);
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
  clib_assert(false, "Unreachable: table overrun: %zu elements", ht->cap);
}

void ht_insert_bytes(HashTable *ht, u64 hash, Bytes bytes) {
  clib_assert((u64)bytes.data < (1L << 48), "pointer out of limit!");
  clib_assert(bytes.size < U16_MAX, "size out of limit!");
  ht_insert(
      ht, hash,
      (HashValue){.bytes.size = bytes.size, .bytes.data = (u64)bytes.data});
}

void ht_insert_copy(HashTable *ht, u64 hash, usize size, const void *value) {
  u8 *buffer = arena_alloc(ht->arena, size);
  memcpy(buffer, value, size);
  ht_insert(ht, hash, (HashValue){.ptr = buffer});
}

bool ht_try_get(HashTable *ht, u64 hash, HashValue *value) {
  usize idx = hash % ht->cap;
  if (ht->nodes[idx].key && ht->nodes[idx].key == hash) {
    *value = ht->nodes[idx].value;
    return true;
  }
  for (usize i = 0; i < ht->cap; i++) {
    idx = (idx + i * i) % ht->cap;
    if (ht->nodes[idx].key && ht->nodes[idx].key == hash) {
      *value = ht->nodes[idx].value;
      return true;
    }
  }
  for (usize i = 0; i < ht->cap; i++) {
    if (ht->nodes[i].key || ht->nodes[i].key == hash) {
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
  HashValue value = {0};
  clib_assert_return(ht_try_get(ht, hash, &value), false);
  bytes->size = value.bytes.size;
  bytes->data = (u8 *)value.bytes.data;
  return true;
}

Bytes ht_get_bytes(HashTable *ht, u64 hash) {
  Bytes bytes = {0};
  clib_assert(ht_try_get_bytes(ht, hash, &bytes),
              "Value was not in hashtable: %lx", hash);
  return bytes;
}

bool ht_contains(HashTable *ht, u64 hash) {
  usize idx = hash % ht->cap;
  if (ht->nodes[idx].key && ht->nodes[idx].key == hash) {
    return true;
  }
  for (usize i = 0; i < ht->cap; i++) {
    idx = (idx + i * i) % ht->cap;
    if (ht->nodes[idx].key && ht->nodes[idx].key == hash) {
      return true;
    }
  }
  for (usize i = 0; i < ht->cap; i++) {
    if (ht->nodes[i].key || ht->nodes[i].key == hash) {
      return true;
    }
  }
  return false;
}
