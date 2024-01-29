#include "hashmap.h"

#include "core/arena.h"
#include "core/assert.h"
#include "core/defines.h"
#include "type/integer.h"

#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

#define HM_DEFAULT_SIZE 8
#define HM_DELETED_HASH 0xdeaddeaddeaddead

////////////////////////////////////////////////////////////////////////////

static bool hm_insert(HashMap *hm, u64 hash, HashValue value) {
  if (hash == 0) {
    hash = u64_hash(hash);
  }
  if (hm->cap <= hm->count + hm->deleted) {
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

////////////////////////////////////////////////////////////////////////////

HashMap hm_create(Arena *arena) {
  HashMap hm = {0};
  hm.arena = arena;
  return hm;
}

HashMap hm_with_size(Arena *arena, usize size) {
  HashMap hm = {0};
  hm.arena = arena;
  hm.cap = usize_max(size, HM_DEFAULT_SIZE);
  hm.nodes = arena_calloc_chunk(arena, hm.cap * sizeof(hm.nodes[0]));
  return hm;
}

void hm_clear(HashMap *hm) {
  hm->count = 0;
  memset(hm->nodes, 0, hm->cap * sizeof(hm->nodes[0]));
}

HashMap hm_copy(HashMap *hm, Arena *arena) {
  HashMap new = hm_with_size(arena, hm->count * 2);
  for (size_t i = 0; i < hm->cap; i++) {
    if (hm->nodes[i].key || hm->nodes[i].key != HM_DELETED_HASH) {
      hm_insert(&new, hm->nodes[i].key, hm->nodes[i].value);
    }
  }
  return new;
}

void hm_resize(HashMap *hm, usize size) {
  if (size < hm->cap) {
    return;
  }
  usize old_cap = hm->cap;
  HashNode *old_nodes = hm->nodes;

  hm->cap = size == 0 ? HM_DEFAULT_SIZE : size;
  hm->nodes = arena_calloc_chunk(hm->arena, hm->cap * sizeof(hm->nodes[0]));

  hm->count = 0;
  hm->deleted = 0;
  for (usize i = 0; i < old_cap; ++i) {
    if (old_nodes[i].key || hm->nodes[i].key != HM_DELETED_HASH) {
      hm_insert(hm, old_nodes[i].key, old_nodes[i].value);
    }
  }
  arena_free_chunk(hm->arena, old_nodes);
}

void hm_reserve(HashMap *hm, usize size) {
  const usize required_size = hm->count + size;
  if (required_size < hm->cap) {
    return;
  }
  usize new_size = hm->cap == 0 ? HM_DEFAULT_SIZE : hm->cap;
  while (new_size < required_size) {
    new_size *= 2;
  }
  hm_resize(hm, new_size);
}

////////////////////////////////////////////////////////////////////////////

#define HM_INSERT_IMPL(T, TYPE)                                                \
  bool hm_insert_##T(HashMap *hm, u64 hash, T value) {                         \
    if (hm->type != HM_NONE) {                                                 \
      clib_assert(hm->type == TYPE, "HashMap stores different type: %s", #T);  \
    } else {                                                                   \
      hm->type = TYPE;                                                         \
    }                                                                          \
    return hm_insert(hm, hash, (HashValue){.as.T = value});                    \
  }

HM_INSERT_IMPL(f32, HM_F32)
HM_INSERT_IMPL(f64, HM_F64)
HM_INSERT_IMPL(i32, HM_I32)
HM_INSERT_IMPL(u32, HM_U32)
HM_INSERT_IMPL(i64, HM_I64)
HM_INSERT_IMPL(u64, HM_U64)

typedef void *ptr;
HM_INSERT_IMPL(ptr, HM_PTR)

void hm_update(HashMap *hm, HashMap *other) {
  hm_reserve(hm, other->count);
  for (usize i = 0; i < other->cap; ++i) {
    if (other->nodes[i].key) {
      hm_insert(hm, other->nodes[i].key, other->nodes[i].value);
    }
  }
}

bool hm_remove(HashMap *hm, usize hash) {
  if (hm->count == 0) {
    return false;
  }
  if (hash == 0 || hash == HM_DELETED_HASH) {
    hash = u64_hash(hash);
  }

  usize idx = hash % hm->cap;
  for (usize i = 0; i < hm->cap; i++) {
    if (hm->nodes[idx].key && hm->nodes[idx].key == hash) {
      hm->nodes[idx].key = HM_DELETED_HASH;
      hm->count--;
      hm->deleted++;
      return true;
    }
    idx = (idx + i * i) % hm->cap;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////

const HashValue *hm_get(const HashMap *hm, u64 hash) {
  return hm_get_mut(hm, hash);
}

HashValue *hm_get_mut(const HashMap *hm, u64 hash) {
  if (hm->count == 0) {
    return NULL;
  }
  if (hash == 0 || hash == HM_DELETED_HASH) {
    hash = u64_hash(hash);
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

////////////////////////////////////////////////////////////////////////////
