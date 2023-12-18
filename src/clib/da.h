#ifndef __CLIB_DA_H__
#define __CLIB_DA_H__

#include <stdlib.h>

// TODO da_reverse
// TODO da_copy
// TODO da_clear
// TODO da_filter

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
    for (size_t i = 0; i < (count); i++) {                                     \
      (list)->items[(list)->len + i] = (_items)[i];                            \
    }                                                                          \
  } while (0)

#define da_free(list)                                                          \
  do {                                                                         \
    free((list)->items);                                                       \
    (list)->len = 0;                                                           \
    (list)->cap = 1;                                                           \
  } while (0)

#define da_map(list, map)                                                      \
  do {                                                                         \
    for (size_t i = 0; i < (list)->len; i++) {                                 \
      (list)->items[i] = map((list)->items[i]);                                \
    }                                                                          \
  } while (0)

#define da_filter(list, filter)                                                \
  do {                                                                         \
    size_t count = 0;                                                          \
    for (size_t i = 0; i < (list)->len; i++) {                                 \
      if (filter((list)->items[i])) {                                          \
        (list)->items[count++] = (list)->items[i];                             \
      }                                                                        \
    }                                                                          \
    (list)->len = count;                                                       \
  } while (0)

#define da_sort(list, sort)                                                    \
  do {                                                                         \
    qsort((list)->items, (list)->len, sizeof((list)->items[0]), sort);         \
  } while (0)

#define da_reverse(list)                                                       \
  do {                                                                         \
    da_reserve((list), 1);                                                     \
    for (size_t i = 0; i < (list)->len - i - 1; i++) {                         \
      (list)->items[(list)->len] = (list)->items[i];                           \
      (list)->items[i] = (list)->items[(list)->len - i - 1];                   \
      (list)->items[(list)->len - i - 1] = (list)->items[(list)->len];         \
    }                                                                          \
  } while (0)

#endif // !__CLIB_DA_H__
