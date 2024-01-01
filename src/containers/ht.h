#ifndef __CLIB_HT_H__
#define __CLIB_HT_H__

#include "clib/arena.h"
#include "datatypes/datatypes.h"
#include <sys/cdefs.h>

#define STATIC_ASSERT(COND, MSG)                                               \
  typedef char static_assertion_##MSG[(COND) ? 1 : -1]

typedef union {
  struct {
    u16 size;
    u64 data : 48;
  } bytes;
  u64 u64;
  void *ptr;
} HashValue;

_Static_assert(sizeof(HashValue) == 8, "HashValue is too big");

typedef struct {
  u64 key;
  HashValue value;
  bool occupied;
} HashNode;

typedef struct {
  Arena arena;
  usize cap;
  usize count;
  HashNode *nodes;
} HashTable;

void ht_free(HashTable *ht);

void ht_insert(HashTable *ht, u64 hash, HashValue value);
void ht_insert_bytes(HashTable *ht, u64 hash, Bytes bytes);
void ht_insert_copy(HashTable *ht, u64 hash, usize size, const void *value);

bool ht_try_get(HashTable *ht, u64 hash, HashValue *value);
HashValue ht_get(HashTable *ht, u64 hash);
bool ht_try_get_bytes(HashTable *ht, u64 hash, Bytes *slice);
Bytes ht_get_bytes(HashTable *ht, u64 hash);

#endif // !__CLIB_HT_H__
