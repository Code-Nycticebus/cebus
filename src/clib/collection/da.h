/* DOCUMENTATION
### Usage
Create a new dynamic array with:
```c
Arena arena = {0};
DA(int) vec = {0};
da_init(&vec, &arena);
```

Then you can push elements to the dynamic array.
```c
da_push(&vec, 69);
da_push(&vec, 420);
```

Then you can push elements to the dynamic array.
```c
da_push(&vec, 69);
da_push(&vec, 420);
```
*/

#ifndef __CLIB_DA_H__
#define __CLIB_DA_H__

#include "clib/core/arena.h"   // IWYU pragma: export
#include "clib/core/defines.h" // IWYU pragma: export
#include "clib/core/sorting.h" // IWYU pragma: export

///////////////////////////////////////////////////////////////////////////////

#define DA(T)                                                                  \
  struct {                                                                     \
    usize cap;                                                                 \
    usize len;                                                                 \
    usize _size;                                                               \
    Arena *arena;                                                              \
    T *items;                                                                  \
  }

#define da_first(list) (list)->items[0]
#define da_last(list) (list)->items[(list)->len - 1]
#define da_pop(list) (list)->items[--(list)->len]
#define da_empty(list) (!(list)->len)
#define da_clear(list) ((list)->len = 0)

///////////////////////////////////////////////////////////////////////////////

#define da_init(list, _arena)                                                  \
  do {                                                                         \
    (list)->len = 0;                                                           \
    (list)->cap = 0;                                                           \
    (list)->arena = _arena;                                                    \
    (list)->items = NULL;                                                      \
    (list)->_size = sizeof(*(list)->items);                                    \
  } while (0)

#define da_init_list(list, _arena, count, array)                               \
  do {                                                                         \
    (list)->len = count;                                                       \
    (list)->cap = 0;                                                           \
    (list)->arena = _arena;                                                    \
    (list)->items = NULL;                                                      \
    (list)->_size = sizeof(*(list)->items);                                    \
    da_resize(list, count);                                                    \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                          \
      (list)->items[__e_i] = (array)[__e_i];                                   \
    }                                                                          \
  } while (0)

#define da_copy(src, dest)                                                     \
  do {                                                                         \
    da_resize((dest), (src)->len);                                             \
    for (usize __c_i = 0; __c_i < (src)->len; __c_i++) {                       \
      (dest)->items[__c_i] = (src)->items[__c_i];                              \
    }                                                                          \
    (dest)->len = (src)->len;                                                  \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#define da_resize(list, size)                                                  \
  do {                                                                         \
    if (size < (list)->cap) {                                                  \
      break;                                                                   \
    }                                                                          \
    (list)->cap = size;                                                        \
    (list)->items = arena_realloc_chunk((list)->arena, (list)->items,          \
                                        (list)->cap * (list)->_size);          \
  } while (0)

#define da_reserve(list, size)                                                 \
  do {                                                                         \
    const usize __rs = (list)->len + size;                                     \
    if (__rs < (list)->cap) {                                                  \
      break;                                                                   \
    }                                                                          \
    usize __ns = (list)->cap == 0 ? 5 : (list)->cap;                           \
    while (__ns < __rs) {                                                      \
      __ns *= 2;                                                               \
    }                                                                          \
    da_resize(list, __ns);                                                     \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#define da_push(list, item)                                                    \
  do {                                                                         \
    da_reserve((list), (list)->len + 1);                                       \
    (list)->items[(list)->len++] = (item);                                     \
  } while (0)

#define da_extend(list, count, _items)                                         \
  do {                                                                         \
    da_reserve((list), (count));                                               \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                          \
      (list)->items[(list)->len + __e_i] = (_items)[__e_i];                    \
    }                                                                          \
    (list)->len += count;                                                      \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

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

#define da_filter_ctx(src, dest, filter, ctx)                                  \
  do {                                                                         \
    da_reserve((dest), (src)->len);                                            \
    usize __f_count = 0;                                                       \
    for (usize __f_i = 0; __f_i < (src)->len; __f_i++) {                       \
      if (filter((ctx), (src)->items[__f_i])) {                                \
        (dest)->items[__f_count++] = (src)->items[__f_i];                      \
      }                                                                        \
    }                                                                          \
    (dest)->len = __f_count;                                                   \
  } while (0)

#define da_sort(src, dest, sort)                                               \
  do {                                                                         \
    da_reserve((dest), (src)->len);                                            \
    quicksort((src)->items, (dest)->items, sizeof((src)->items[0]),            \
              (src)->len, sort);                                               \
  } while (0)

#define da_sort_ctx(src, dest, sort, ctx)                                      \
  do {                                                                         \
    da_reserve((dest), (src)->len);                                            \
    quicksort_ctx((src)->items, (dest)->items, sizeof((src)->items[0]),        \
                  (src)->len, sort, ctx);                                      \
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

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CLIB_DA_H__ */
