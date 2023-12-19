#ifndef __CLIB_DA_H__
#define __CLIB_DA_H__

#include <stdlib.h>

#define DA(T)                                                                  \
  struct {                                                                     \
    size_t cap;                                                                \
    size_t len;                                                                \
    T *items;                                                                  \
  }

#define da_last(list) (list)->items[(list)->len - 1]
#define da_empty(list) (!(list)->len)

#define da_init(list, size)                                                    \
  do {                                                                         \
    (list)->cap = (size);                                                      \
    (list)->items = malloc((size) * sizeof((list)->items[0]));                 \
    (list)->len = 0;                                                           \
  } while (0)

#define da_clear(list)                                                         \
  do {                                                                         \
    (list)->len = 0;                                                           \
  } while (0)

#define da_reserve(list, size)                                                 \
  do {                                                                         \
    if (!((list)->len + size < (list)->cap)) {                                 \
      (list)->cap += size;                                                     \
      (list)->items =                                                          \
          realloc((list)->items, (list)->cap * sizeof((list)->items[0]));      \
    }                                                                          \
  } while (0)

#define da_push(list, item)                                                    \
  do {                                                                         \
    da_reserve((list), 1);                                                     \
    (list)->items[(list)->len++] = (item);                                     \
  } while (0)

#define da_extend(list, count, _items)                                         \
  do {                                                                         \
    da_reserve((list), (count));                                               \
    for (size_t __e_i = 0; __e_i < (count); __e_i++) {                         \
      (list)->items[(list)->len + __e_i] = (_items)[__e_i];                    \
    }                                                                          \
  } while (0)

#define da_free(list)                                                          \
  do {                                                                         \
    free((list)->items);                                                       \
    (list)->len = 0;                                                           \
    (list)->cap = 1;                                                           \
  } while (0)

#define da_map(src, dest, map)                                                 \
  do {                                                                         \
    da_reserve((dest), (src)->len);                                            \
    for (size_t __m_i = 0; __m_i < (src)->len; __m_i++) {                      \
      (dest)->items[__m_i] = map((src)->items[__m_i]);                         \
    }                                                                          \
    (dest)->len = (src)->len;                                                  \
  } while (0)

#define da_filter(list, filter)                                                \
  do {                                                                         \
    size_t __f_count = 0;                                                      \
    for (size_t __f_i = 0; __f_i < (list)->len; __f_i++) {                     \
      if (filter((list)->items[__f_i])) {                                      \
        (list)->items[__f_count++] = (list)->items[__f_i];                     \
      }                                                                        \
    }                                                                          \
    (list)->len = __f_count;                                                   \
  } while (0)

#define da_sort(list, sort)                                                    \
  do {                                                                         \
    qsort((list)->items, (list)->len, sizeof((list)->items[0]), sort);         \
  } while (0)

#define da_reverse(list)                                                       \
  do {                                                                         \
    da_reserve((list), 1);                                                     \
    for (size_t __r_i = 0; __r_i < (list)->len - __r_i - 1; __r_i++) {         \
      (list)->items[(list)->len] = (list)->items[__r_i];                       \
      (list)->items[__r_i] = (list)->items[(list)->len - __r_i - 1];           \
      (list)->items[(list)->len - __r_i - 1] = (list)->items[(list)->len];     \
    }                                                                          \
  } while (0)

#define da_copy(src, dest)                                                     \
  do {                                                                         \
    da_reserve((dest), (src)->len);                                            \
    for (size_t __c_i = 0; __c_i < (src)->len; __c_i++) {                      \
      (dest)->items[__c_i] = (src)->items[__c_i];                              \
    }                                                                          \
    (dest)->len = (src)->len;                                                  \
  } while (0)

#endif /* !__CLIB_DA_H__ */
