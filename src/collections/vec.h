#ifndef __CLIB_DA_H__
#define __CLIB_DA_H__

/* DOCUMENTATION
## Usage:
Create a new Vec with:
```c
Arena arena = {0};
VEC(int) vec = {0};
vec_init(&vec, &arena);
```

Then you can push elements to the vector.
```c
vec_push(&vec, 69);
vec_push(&vec, 420);
```
*/

#include "core/arena.h"     // IWYU pragma: export
#include "core/defines.h"   // IWYU pragma: export
#include "core/sorting.h"   // IWYU pragma: export
#include "types/integers.h" // IWYU pragma: export

#define VEC(T)                                                                 \
  struct {                                                                     \
    usize cap;                                                                 \
    usize len;                                                                 \
    Arena *arena;                                                              \
    T *items;                                                                  \
  }

#define vec_init(list, _arena)                                                 \
  do {                                                                         \
    (list)->len = 0;                                                           \
    (list)->cap = 0;                                                           \
    (list)->arena = _arena;                                                    \
    (list)->items = NULL;                                                      \
  } while (0)

#define vec_init_list(list, _arena, count, array)                              \
  do {                                                                         \
    (list)->len = count;                                                       \
    (list)->cap = count;                                                       \
    (list)->arena = _arena;                                                    \
    (list)->items =                                                            \
        arena_alloc_chunk(_arena, count * sizeof((list)->items[0]));           \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                          \
      (list)->items[__e_i] = (array)[__e_i];                                   \
    }                                                                          \
  } while (0)

#define vec_first(list) (list)->items[0]
#define vec_last(list) (list)->items[(list)->len - 1]
#define vec_pop(list) (list)->items[--(list)->len]
#define vec_empty(list) (!(list)->len)

#define vec_clear(list)                                                        \
  do {                                                                         \
    (list)->len = 0;                                                           \
  } while (0)

#define vec_reserve(list, size)                                                \
  do {                                                                         \
    if (!(size < (list)->cap)) {                                               \
      (list)->cap = usize_max(size, 10);                                       \
      (list)->items =                                                          \
          arena_realloc_chunk((list)->arena, (list)->items,                    \
                              (list)->cap * sizeof((list)->items[0]));         \
    }                                                                          \
  } while (0)

#define vec_push(list, item)                                                   \
  do {                                                                         \
    vec_reserve((list), (list)->len + 1);                                      \
    (list)->items[(list)->len++] = (item);                                     \
  } while (0)

#define vec_extend(list, count, _items)                                        \
  do {                                                                         \
    vec_reserve((list), (count));                                              \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                          \
      (list)->items[(list)->len + __e_i] = (_items)[__e_i];                    \
    }                                                                          \
    (list)->len += count;                                                      \
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

#define vec_filter_ctx(src, dest, filter, ctx)                                 \
  do {                                                                         \
    vec_reserve((dest), (src)->len);                                           \
    usize __f_count = 0;                                                       \
    for (usize __f_i = 0; __f_i < (src)->len; __f_i++) {                       \
      if (filter((ctx), (src)->items[__f_i])) {                                \
        (dest)->items[__f_count++] = (src)->items[__f_i];                      \
      }                                                                        \
    }                                                                          \
    (dest)->len = __f_count;                                                   \
  } while (0)

#define vec_sort(src, dest, sort)                                              \
  do {                                                                         \
    vec_reserve((dest), (src)->len);                                           \
    quicksort((src)->items, (dest)->items, sizeof((src)->items[0]),            \
              (src)->len, sort);                                               \
  } while (0)

#define vec_sort_ctx(src, dest, sort, ctx)                                     \
  do {                                                                         \
    vec_reserve((dest), (src)->len);                                           \
    quicksort_ctx((src)->items, (dest)->items, sizeof((src)->items[0]),        \
                  (src)->len, sort, ctx);                                      \
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
