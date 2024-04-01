#include "hashmap.h"

#include "clib/core/assert.h"
#include "clib/type/integer.h"

#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////

#define HM_TYPES(DO)                                                           \
  DO(f32)                                                                      \
  DO(f64)                                                                      \
  DO(i8)                                                                       \
  DO(u8)                                                                       \
  DO(i16)                                                                      \
  DO(u16)                                                                      \
  DO(i32)                                                                      \
  DO(u32)                                                                      \
  DO(i64)                                                                      \
  DO(u64)                                                                      \
  DO(usize)

#define HM_DECLARE_ENUM(T) HM_TYPE_##T,

typedef enum {
  HM_NONE,
  HM_PTR,
  HM_CONST_PTR,
  HM_TYPES(HM_DECLARE_ENUM)
} HashTypes;

#undef HM_DECLARE_ENUM

#define HM_DECLARE_MEMBER(T) T T;

typedef struct {
  union {
    void *ptr;
    const void *const_ptr;
    HM_TYPES(HM_DECLARE_MEMBER)
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
#define HM_DELETED_HASH ((u64)0xdeaddeaddeaddead)

////////////////////////////////////////////////////////////////////////////

static bool hm_insert(HashMap *hm, u64 hash, HashValue value) {
  if (hash == 0 || hash == HM_DELETED_HASH) {
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
    if (hm->nodes[idx].key == 0) {
      return NULL;
    }
    if (hm->nodes[idx].key == hash) {
      return &hm->nodes[idx].value;
    }
    idx = (idx + i * i) % hm->cap;
  }
  return NULL;
}

static const char *hm_type(HashTypes type) {
#define RETURN_STR(T)                                                          \
  case HM_TYPE_##T:                                                            \
    return #T;

  switch (type) {
  case HM_PTR:
    return "HM_PTR";
  case HM_CONST_PTR:
    return "HM_CONST_PTR";
  case HM_NONE:
    return "HM_NONE";

    HM_TYPES(RETURN_STR)
  }
#undef RETURN_STR
  clib_assert(false, "UNREACHABLE: %d", type);
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
    if (hm->nodes[i].key && hm->nodes[i].key != HM_DELETED_HASH) {
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
    if (old_nodes[i].key && hm->nodes[i].key != HM_DELETED_HASH) {
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
    if (other->nodes[i].key && other->nodes[i].key != HM_DELETED_HASH) {
      hm_insert(hm, other->nodes[i].key, other->nodes[i].value);
    }
  }
}

bool hm_remove(HashMap *hm, u64 hash) {
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
    if (hm->type != HM_NONE && (hm->type != T)) {                              \
      clib_log_warning("HashMap Type Error: expected '%s' got '%s'",           \
                       hm_type(hm->type), #T);                                 \
      return NULL;                                                             \
    }                                                                          \
  } while (0)

#define HM_INSERT_IMPL(T)                                                      \
  bool hm_insert_##T(HashMap *hm, u64 hash, T value) {                         \
    TYPE_CHECK(hm, HM_TYPE_##T);                                               \
    hm->type = HM_TYPE_##T;                                                    \
    return hm_insert(hm, hash, (HashValue){.as.T = value});                    \
  }

HM_TYPES(HM_INSERT_IMPL)

#undef HM_INSERT_IMPL

bool hm_insert_mut_ptr(HashMap *hm, u64 hash, void *value) {
  TYPE_CHECK(hm, HM_PTR);
  hm->type = HM_PTR;
  return hm_insert(hm, hash, (HashValue){.as.ptr = value});
}

bool hm_insert_ptr(HashMap *hm, u64 hash, const void *value) {
  TYPE_CHECK(hm, HM_CONST_PTR);
  hm->type = HM_CONST_PTR;
  return hm_insert(hm, hash, (HashValue){.as.const_ptr = value});
}

////////////////////////////////////////////////////////////////////////////

#define HM_GET_MUT_IMPL(T)                                                     \
  T *hm_get_##T##_mut(const HashMap *hm, u64 hash) {                           \
    TYPE_CHECK(hm, HM_TYPE_##T);                                               \
    HashValue *value = hm_get(hm, hash);                                       \
    return value ? &value->as.T : NULL;                                        \
  }

HM_TYPES(HM_GET_MUT_IMPL)

#undef HM_GET_MUT_IMPL

void *hm_get_ptr_mut(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_PTR);
  HashValue *value = hm_get(hm, hash);
  return value ? value->as.ptr : NULL;
}

///////////////////////////////////////////////////////////////////////////////

#define HM_GET_IMPL(T)                                                         \
  const T *hm_get_##T(const HashMap *hm, u64 hash) {                           \
    return hm_get_##T##_mut(hm, hash);                                         \
  }

HM_TYPES(HM_GET_IMPL)

#undef HM_GET_IMPL

const void *hm_get_ptr(const HashMap *hm, u64 hash) {
  TYPE_CHECK(hm, HM_CONST_PTR && hm->type != HM_PTR);
  HashValue *value = hm_get(hm, hash);
  return value ? value->as.const_ptr : NULL;
}

///////////////////////////////////////////////////////////////////////////////
