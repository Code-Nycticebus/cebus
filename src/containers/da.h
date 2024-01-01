#ifndef __CLIB_DA_H__
#define __CLIB_DA_H__

#include "clib/defines.h" // IWYU pragma: export
#include <stdlib.h>       // IWYU pragma: export

#define DA(T)                                                                  \
  struct {                                                                     \
    Arena arena;                                                               \
    usize cap;                                                                 \
    usize len;                                                                 \
    T *items;                                                                  \
  }

#define da_first(list) (list)->items[0]
#define da_last(list) (list)->items[(list)->len - 1]
#define da_empty(list) (!(list)->len)

#define da_clear(list)                                                         \
  do {                                                                         \
    (list)->len = 0;                                                           \
  } while (0)

#define da_reserve(list, size)                                                 \
  do {                                                                         \
    if (!((list)->len + size < (list)->cap)) {                                 \
      (list)->cap += usize_max(size, 10);                                      \
      (list)->items =                                                          \
          arena_temp_realloc(&(list)->arena, (list)->items,                    \
                             (list)->cap * sizeof((list)->items[0]));          \
    }                                                                          \
  } while (0)

#define da_push(list, item)                                                    \
  do {                                                                         \
    da_reserve((list), 1);                                                     \
    (list)->items[(list)->len++] = (item);                                     \
  } while (0)

#define da_pop(list) (list)->items[--(list)->len]

#define da_extend(list, count, _items)                                         \
  do {                                                                         \
    da_reserve((list), (count));                                               \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                          \
      (list)->items[(list)->len + __e_i] = (_items)[__e_i];                    \
    }                                                                          \
  } while (0)

#define da_free(list)                                                          \
  do {                                                                         \
    arena_free(&(list)->arena);                                                \
    (list)->len = 0;                                                           \
    (list)->cap = 1;                                                           \
  } while (0)

#define da_map(src, dest, map)                                                 \
  do {                                                                         \
    da_reserve((dest), (src)->len);                                            \
    for (usize __m_i = 0; __m_i < (src)->len; __m_i++) {                       \
      (dest)->items[__m_i] = map((src)->items[__m_i]);                         \
    }                                                                          \
    (dest)->len = (src)->len;                                                  \
  } while (0)

#define da_filter(src, dest, filter)                                           \
  do {                                                                         \
    da_reserve((dest), (src)->len);                                            \
    usize __f_count = 0;                                                       \
    for (usize __f_i = 0; __f_i < (src)->len; __f_i++) {                       \
      if (filter((src)->items[__f_i])) {                                       \
        (dest)->items[__f_count++] = (src)->items[__f_i];                      \
      }                                                                        \
    }                                                                          \
    (dest)->len = __f_count;                                                   \
  } while (0)

#define da_sort(list, sort)                                                    \
  do {                                                                         \
    qsort((list)->items, (list)->len, sizeof((list)->items[0]), sort);         \
  } while (0)

#define da_reverse(list)                                                       \
  do {                                                                         \
    da_reserve((list), 1);                                                     \
    for (usize __r_i = 0; __r_i < (list)->len - __r_i - 1; __r_i++) {          \
      (list)->items[(list)->len] = (list)->items[__r_i];                       \
      (list)->items[__r_i] = (list)->items[(list)->len - __r_i - 1];           \
      (list)->items[(list)->len - __r_i - 1] = (list)->items[(list)->len];     \
    }                                                                          \
  } while (0)

#define da_copy(src, dest)                                                     \
  do {                                                                         \
    da_reserve((dest), (src)->len);                                            \
    for (usize __c_i = 0; __c_i < (src)->len; __c_i++) {                       \
      (dest)->items[__c_i] = (src)->items[__c_i];                              \
    }                                                                          \
    (dest)->len = (src)->len;                                                  \
  } while (0)

#endif /* !__CLIB_DA_H__ */
