#ifndef __CLIB_HT_H__
#define __CLIB_HT_H__

#include "core/arena.h"
#include "core/defines.h"

typedef struct {
  union {
    f32 f32;
    f64 f64;
    i32 i32;
    u32 u32;
    i64 i64;
    u64 u64;
    void *ptr;
  } as;
} HashValue;

typedef struct {
  u64 key;
  HashValue value;
} HashNode;

typedef struct {
  usize cap;
  usize count;
  Arena *arena;
  HashNode *nodes;
} HashMap;

HashMap hm_create(Arena *arena, usize size);

void hm_insert(HashMap *hm, u64 hash, HashValue value);
HashValue *hm_get(const HashMap *hm, u64 hash);

#endif // !__CLIB_HT_H__
