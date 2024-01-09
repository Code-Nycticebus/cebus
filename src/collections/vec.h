#ifndef __CLIB_DA_H__
#define __CLIB_DA_H__

#include "clib/asserts.h"
#include "core/defines.h" // IWYU pragma: export
#include <stdlib.h>       // IWYU pragma: export

#define VEC(T)                                                                 \
  struct {                                                                     \
    usize cap;                                                                 \
    usize len;                                                                 \
    T *items;                                                                  \
  }

#define vec_init(list, _cap, _arena)                                           \
  do {                                                                         \
    (list)->len = 0;                                                           \
    (list)->cap = _cap;                                                        \
    (list)->items =                                                            \
        arena_alloc_chunk(_arena, _cap * sizeof((list)->items[0]));            \
  } while (0)

#define vec_first(list) (list)->items[0]
#define vec_last(list) (list)->items[(list)->len - 1]
#define vec_empty(list) (!(list)->len)

#define vec_clear(list)                                                        \
  do {                                                                         \
    (list)->len = 0;                                                           \
  } while (0)

#define vec_reserve(list, size)                                                \
  do {                                                                         \
    clib_assert_debug(size <= SIZE_MAX - (list)->cap, "integer overflow");     \
    if (!((list)->len + size < (list)->cap)) {                                 \
      (list)->cap = usize_max((list)->cap + size, 10);                         \
      (list)->items = arena_realloc_chunk(                                     \
          (list)->items, (list)->cap * sizeof((list)->items[0]));              \
    }                                                                          \
  } while (0)

#define vec_push(list, item)                                                   \
  do {                                                                         \
    vec_reserve((list), 1);                                                    \
    (list)->items[(list)->len++] = (item);                                     \
  } while (0)

#define vec_pop(list) (list)->items[--(list)->len]

#define vec_extend(list, count, _items)                                        \
  do {                                                                         \
    vec_reserve((list), (count));                                              \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                          \
      (list)->items[(list)->len + __e_i] = (_items)[__e_i];                    \
    }                                                                          \
  } while (0)

#define vec_map(src, dest, map)                                                \
  do {                                                                         \
    vec_reserve((dest), (src)->len);                                           \
    for (usize __m_i = 0; __m_i < (src)->len; __m_i++) {                       \
      (dest)->items[__m_i] = map((src)->items[__m_i]);                         \
    }                                                                          \
    (dest)->len = (src)->len;                                                  \
  } while (0)

#define vec_filter(src, dest, filter)                                          \
  do {                                                                         \
    vec_reserve((dest), (src)->len);                                           \
    usize __f_count = 0;                                                       \
    for (usize __f_i = 0; __f_i < (src)->len; __f_i++) {                       \
      if (filter((src)->items[__f_i])) {                                       \
        (dest)->items[__f_count++] = (src)->items[__f_i];                      \
      }                                                                        \
    }                                                                          \
    (dest)->len = __f_count;                                                   \
  } while (0)

#define vec_sort(list, sort)                                                   \
  do {                                                                         \
    qsort((list)->items, (list)->len, sizeof((list)->items[0]), sort);         \
  } while (0)

#define vec_reverse(list)                                                      \
  do {                                                                         \
    vec_reserve((list), 1);                                                    \
    for (usize __r_i = 0; __r_i < (list)->len - __r_i - 1; __r_i++) {          \
      (list)->items[(list)->len] = (list)->items[__r_i];                       \
      (list)->items[__r_i] = (list)->items[(list)->len - __r_i - 1];           \
      (list)->items[(list)->len - __r_i - 1] = (list)->items[(list)->len];     \
    }                                                                          \
  } while (0)

#define vec_copy(src, dest)                                                    \
  do {                                                                         \
    vec_reserve((dest), (src)->len);                                           \
    for (usize __c_i = 0; __c_i < (src)->len; __c_i++) {                       \
      (dest)->items[__c_i] = (src)->items[__c_i];                              \
    }                                                                          \
    (dest)->len = (src)->len;                                                  \
  } while (0)

#endif /* !__CLIB_DA_H__ */
