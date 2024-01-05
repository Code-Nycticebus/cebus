#ifndef __CLIB_HT_H__
#define __CLIB_HT_H__

#include "clib/arena.h"
#include "core/defines.h"
#include <sys/cdefs.h>

typedef union {
  u64 u64;
  void *ptr;
} HashValue;

// _Static_assert(sizeof(HashValue) == 8, "HashValue is too big");

typedef struct {
  u64 key;
  HashValue value;
} HashNode;

// _Static_assert(sizeof(HashNode) == 16, "HashValue is too big");

typedef struct {
  usize cap;
  usize count;
  HashNode *nodes;
} HashTable;

HashTable ht_create(Arena *arena, usize size);

void ht_insert(HashTable *ht, u64 hash, HashValue value);
HashNode *ht_get(HashTable *ht, u64 hash);

#endif // !__CLIB_HT_H__
