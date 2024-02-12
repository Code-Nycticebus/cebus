#include "hashmap.h"

#include "core/arena.h"
#include "core/assert.h"
#include "core/defines.h"
#include "type/integer.h"

#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

typedef enum {
  HM_NONE,
  HM_F32,
  HM_F64,
  HM_I32,
  HM_U32,
  HM_I64,
  HM_U64,
  HM_PTR,
  HM_CONST_PTR,
} HashTypes;

typedef struct {
  union {
    f32 f32;
    f64 f64;
    i32 i32;
    u32 u32;
    i64 i64;
    u64 u64;
    void *ptr;
    const void *const_ptr;
  } as;
} HashValue;

typedef struct {
  u64 key;
  HashValue value;
} HashNode;

struct HashMap {
  HashTypes type;
  usize cap;
  usize count;
  usize deleted;
  Arena *arena;
  HashNode *nodes;
};

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

static HashValue *hm_get(const HashMap *hm, u64 hash) {
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

static const char *hm_type(HashTypes type) {
  switch (type) {
  case HM_F32:
    return "HM_F32";
  case HM_F64:
    return "HM_F64";
  case HM_I32:
    return "HM_I32";
  case HM_U32:
    return "HM_U32";
  case HM_I64:
    return "HM_I64";
  case HM_U64:
    return "HM_U64";
  case HM_PTR:
    return "HM_PTR";
  case HM_CONST_PTR:
    return "HM_CONST_PTR";
  case HM_NONE:
    return "HM_NONE";
  }
  // clib_assert(false, "UNREACHABLE: %d", type);
  return NULL;
}

////////////////////////////////////////////////////////////////////////////

HashMap *hm_create(Arena *arena) {
  HashMap *hm = arena_calloc(arena, sizeof(HashMap));
  hm->type = HM_NONE;
  hm->arena = arena;
  return hm;
}

HashMap *hm_with_size(Arena *arena, usize size) {
  HashMap *hm = arena_calloc(arena, sizeof(HashMap));
  hm->type = HM_NONE;
  hm->arena = arena;
  hm->cap = usize_max(size, HM_DEFAULT_SIZE);
  hm->nodes = arena_calloc_chunk(arena, hm->cap * sizeof(hm->nodes[0]));
  return hm;
}

void hm_clear(HashMap *hm) {
  hm->type = HM_NONE;
  hm->count = 0;
  memset(hm->nodes, 0, hm->cap * sizeof(hm->nodes[0]));
}

HashMap *hm_copy(HashMap *hm, Arena *arena) {
  HashMap *new = hm_with_size(arena, hm->count * 2);
  new->type = hm->type;
  for (size_t i = 0; i < hm->cap; i++) {
    if (hm->nodes[i].key || hm->nodes[i].key != HM_DELETED_HASH) {
      hm_insert(new, hm->nodes[i].key, hm->nodes[i].value);
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

#define TYPE_CHECK(hm, T)                                                      \
  do {                                                                         \
    if (hm->type != HM_NONE && hm->type != T) {                                \
      clib_log_warning("HashMap Type Error: expected '%s' got '%s'",           \
                       hm_type(hm->type), #T);                                 \
      return NULL;                                                             \
    }                                                                          \
  } while (0)

bool hm_insert_f32(HashMap *hm, u64 hash, f32 value) {
  TYPE_CHECK(hm, HM_F32);
  hm->type = HM_F32;
  return hm_insert(hm, hash, (HashValue){.as.f32 = value});
}

bool hm_insert_f64(HashMap *hm, u64 hash, f64 value) {
  TYPE_CHECK(hm, HM_F64);
  hm->type = HM_F64;
  return hm_insert(hm, hash, (HashValue){.as.f64 = value});
}

bool hm_insert_i32(HashMap *hm, u64 hash, i32 value) {
  TYPE_CHECK(hm, HM_I32);
  hm->type = HM_I32;
  return hm_insert(hm, hash, (HashValue){.as.i32 = value});
}

bool hm_insert_u32(HashMap *hm, u64 hash, u32 value) {
  TYPE_CHECK(hm, HM_U32);
  hm->type = HM_U32;
  return hm_insert(hm, hash, (HashValue){.as.u32 = value});
}

bool hm_insert_i64(HashMap *hm, u64 hash, i64 value) {
  TYPE_CHECK(hm, HM_I64);
  hm->type = HM_I64;
  return hm_insert(hm, hash, (HashValue){.as.i64 = value});
}

bool hm_insert_u64(HashMap *hm, u64 hash, u64 value) {
  TYPE_CHECK(hm, HM_U64);
  hm->type = HM_U64;
  return hm_insert(hm, hash, (HashValue){.as.u64 = value});
}

bool hm_insert_ptr(HashMap *hm, u64 hash, void *value) {
  TYPE_CHECK(hm, HM_PTR);
  hm->type = HM_PTR;
  return hm_insert(hm, hash, (HashValue){.as.ptr = value});
}

bool hm_insert_const_ptr(HashMap *hm, u64 hash, const void *value) {
  TYPE_CHECK(hm, HM_CONST_PTR);
  hm->type = HM_CONST_PTR;
  return hm_insert(hm, hash, (HashValue){.as.const_ptr = value});
}

////////////////////////////////////////////////////////////////////////////

f32 *hm_get_f32_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_F32);
  return &hm_get(hm, hash)->as.f32;
}
f64 *hm_get_f64_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_F64);
  return &hm_get(hm, hash)->as.f64;
}
i32 *hm_get_i32_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_I32);
  return &hm_get(hm, hash)->as.i32;
}
u32 *hm_get_u32_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_U32);
  return &hm_get(hm, hash)->as.u32;
}
i64 *hm_get_i64_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_I64);
  return &hm_get(hm, hash)->as.i64;
}
u64 *hm_get_u64_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_U64);
  return &hm_get(hm, hash)->as.u64;
}
void **hm_get_ptr_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_PTR);
  return &hm_get(hm, hash)->as.ptr;
}
///////////////////////////////////////////////////////////////////////////////

const f32 *hm_get_f32(const HashMap *hm, u64 hash) {
  return hm_get_f32_mut(hm, hash);
}
const f64 *hm_get_f64(const HashMap *hm, u64 hash) {
  return hm_get_f64_mut(hm, hash);
}
const i32 *hm_get_i32(const HashMap *hm, u64 hash) {
  return hm_get_i32_mut(hm, hash);
}
const u32 *hm_get_u32(const HashMap *hm, u64 hash) {
  return hm_get_u32_mut(hm, hash);
}
const i64 *hm_get_i64(const HashMap *hm, u64 hash) {
  return hm_get_i64_mut(hm, hash);
}
const u64 *hm_get_u64(const HashMap *hm, u64 hash) {
  return hm_get_u64_mut(hm, hash);
}
const void **hm_get_ptr(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_CONST_PTR);
  return &hm_get(hm, hash)->as.const_ptr;
}

///////////////////////////////////////////////////////////////////////////////
