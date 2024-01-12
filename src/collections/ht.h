#ifndef __CLIB_HT_H__
#define __CLIB_HT_H__

#include "clib/arena.h"
#include "core/defines.h"

typedef union {
  f32 f32;
  f64 f64;
  i32 i32;
  u32 u32;
  i64 i64;
  u64 u64;
  void *ptr;
} HashValue;

typedef struct {
  u64 key;
  HashValue value;
} HashNode;

typedef struct {
  usize cap;
  usize count;
  HashNode *nodes;
} HashTable;

HashTable ht_create(Arena *arena, usize size);

void ht_insert(HashTable *ht, u64 hash, HashValue value);
HashNode *ht_get(HashTable *ht, u64 hash);

#endif // !__CLIB_HT_H__
