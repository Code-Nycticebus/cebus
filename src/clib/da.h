#ifndef __CTOOLS_DA_H__
#define __CTOOLS_DA_H__

#include <stdlib.h>

#define DA(T)                                                                  \
  size_t cap;                                                                  \
  size_t len;                                                                  \
  T *items

#define da_init(list, size)                                                    \
  do {                                                                         \
    (list)->cap = (size);                                                      \
    (list)->items = malloc((size) * sizeof((list)->items[0]));                 \
    (list)->len = 0;                                                           \
  } while (0)

#define da_push(list, item)                                                    \
  do {                                                                         \
    if (!((list)->len < (list)->cap)) {                                        \
      (list)->cap *= 1.5F;                                                     \
      (list)->items =                                                          \
          realloc((list)->items, (list)->cap * sizeof((list)->items[0]));      \
    }                                                                          \
    (list)->items[(list)->len++] = item;                                       \
  } while (0)

#define da_free(list)                                                          \
  do {                                                                         \
    free((list)->items);                                                       \
    (list)->len = 0;                                                           \
    (list)->cap = 1;                                                           \
  } while (0)

#endif // !__CTOOLS_DA_H__