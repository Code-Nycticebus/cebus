/* DOCUMENTATION
## Initialization

To start using the dynamic array, you first need to create an instance of
`Arena` for memory management and then initialize the dynamic array with the
specific type.

```c
Arena arena = {0};
DA(int) vec = da_new(&arena);
```

## Adding Elements

Elements can be added to the dynamic array using `da_push`, which automatically
resizes the array if additional space is required.

```c
da_push(&vec, 69);
da_push(&vec, 420);
```

## Accessing Elements

Access the first and last elements with `da_first` and `da_last`. Remove the
last element with `da_pop`. Get any element with `da_get`.

> :warning: These operations do not perform any bounds checks. So make sure your
dynamic array has at least one element in it.

```c
int first = da_first(&vec);
int last = da_last(&vec);
int nth = da_get(&vec, 3);
int popped = da_pop(&vec);
```

## Array Utils

- `da_empty`: Use to check if the array has no elements.
- `da_len`: Get the length of the dynamic array.
- `da_clear`: Reset the length of the array to zero.
- `da_init`:  Initialize dynamic array.
- `da_init_list`: Initialize dynamic array from a array.
- `da_init_static`: Initialize dynamic array from a static array.
- `da_copy`: Duplicate the contents of one dynamic array into another.

## Removing and inserting
> :warning: These operations do not perform any bound checks.

- `da_insert`: Insert a value at a specified index.
- `da_remove`: Remove a value at a specified index.

## Resizing and Reserving Space

- `da_resize`: Adjust the capacity of the array to a specified size. Used for
preallocating space.
- `da_reserve`: Ensure there is enough space for additional elements. Used
before adding multiple elements.

## Manipulating the Array

- `da_extend`: Add multiple elements from another array or list.
- `da_map`: Transform elements of the array into another form and store them in
a destination array.
- `da_map_ctx`: Same but the mapping function also takes a context.
- `da_filter`: Filter dynamic array with a filter function and place it into a
destination.
- `da_filter_ctx`: Filter dynamic array with a filter function, that takes a
`void*` as a context, and place it into a destination.
- `da_sort`: Sort the array using a comparison function.
- `da_reverse`: Reverse the order of elements in the array.
*/

#ifndef __CEBUS_DA_H__
#define __CEBUS_DA_H__

#include "cebus/core/arena.h"   // IWYU pragma: export
#include "cebus/core/defines.h" // IWYU pragma: export

#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////

#define DA(T)                                                                                      \
  struct {                                                                                         \
    usize cap;                                                                                     \
    usize len;                                                                                     \
    Arena *arena;                                                                                  \
    T *items;                                                                                      \
  }

#define da_first(list) (list)->items[0]
#define da_last(list) (list)->items[(list)->len - 1]
#define da_get(list, idx) (list)->items[idx]
#define da_pop(list) (list)->items[--(list)->len]
#define da_empty(list) (!(list)->len)
#define da_len(list) ((list)->len)

#define da_clear(list) ((list)->len = 0)

#define DA_ARG(da) (da)->len, (da)->items

///////////////////////////////////////////////////////////////////////////////

#define da_new(_arena)                                                                             \
  { .arena = (_arena), .items = NULL, }

// depricated
#define da_init(list, _arena)                                                                      \
  do {                                                                                             \
    (list)->len = 0;                                                                               \
    (list)->cap = 0;                                                                               \
    (list)->arena = _arena;                                                                        \
    (list)->items = NULL;                                                                          \
  } while (0)

#define da_init_list(list, _arena, count, array)                                                   \
  do {                                                                                             \
    (list)->len = count;                                                                           \
    (list)->cap = 0;                                                                               \
    (list)->arena = _arena;                                                                        \
    (list)->items = NULL;                                                                          \
    da_resize(list, count);                                                                        \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                                              \
      (list)->items[__e_i] = (array)[__e_i];                                                       \
    }                                                                                              \
  } while (0)

#define da_init_static(list, _arena, ...)                                                          \
  do {                                                                                             \
    da_init_list(list, _arena, ARRAY_LEN((__VA_ARGS__)), (__VA_ARGS__));                           \
  } while (0)

#define da_copy(src, dest)                                                                         \
  do {                                                                                             \
    da_resize((dest), (src)->len);                                                                 \
    for (usize __c_i = 0; __c_i < (src)->len; __c_i++) {                                           \
      (dest)->items[__c_i] = (src)->items[__c_i];                                                  \
    }                                                                                              \
    (dest)->len = (src)->len;                                                                      \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#define da_resize(list, size)                                                                      \
  do {                                                                                             \
    if (size < (list)->cap) {                                                                      \
      break;                                                                                       \
    }                                                                                              \
    (list)->cap = size;                                                                            \
    (list)->items =                                                                                \
        arena_realloc_chunk((list)->arena, (list)->items, (list)->cap * sizeof(*(list)->items));   \
  } while (0)

#define da_reserve(list, size)                                                                     \
  do {                                                                                             \
    const usize __rs = da_len(list) + size;                                                        \
    if (__rs < (list)->cap) {                                                                      \
      break;                                                                                       \
    }                                                                                              \
    usize __ns = (list)->cap == 0 ? 5 : (list)->cap;                                               \
    while (__ns < __rs) {                                                                          \
      __ns *= 2;                                                                                   \
    }                                                                                              \
    da_resize(list, __ns);                                                                         \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#define da_push(list, ...)                                                                         \
  do {                                                                                             \
    da_reserve((list), 1);                                                                         \
    da_get(list, da_len(list)++) = (__VA_ARGS__);                                                  \
  } while (0)

#define da_extend(list, count, ...)                                                                \
  do {                                                                                             \
    da_reserve((list), (count));                                                                   \
    for (usize __e_i = 0; __e_i < (count); __e_i++) {                                              \
      da_get(list, da_len(list) + __e_i) = (__VA_ARGS__)[__e_i];                                   \
    }                                                                                              \
    (list)->len += count;                                                                          \
  } while (0)

#define da_extend_da(list, other)                                                                  \
  do {                                                                                             \
    da_reserve((list), (other)->len);                                                              \
    for (usize __e_i = 0; __e_i < (other)->len; __e_i++) {                                         \
      da_get(list, da_len(list) + __e_i) = (other)->items[__e_i];                                  \
    }                                                                                              \
    (list)->len += (other)->len;                                                                   \
  } while (0)

#define da_insert(list, value, idx)                                                                \
  do {                                                                                             \
    da_reserve(list, 1);                                                                           \
    for (usize __r_i = idx + 1; __r_i < da_len(list) + 1; __r_i++) {                               \
      da_get(list, __r_i) = da_get(list, __r_i - 1);                                               \
    }                                                                                              \
    da_get(list, idx) = value;                                                                     \
    da_len(list)++;                                                                                \
  } while (0)

#define da_remove(list, idx)                                                                       \
  do {                                                                                             \
    for (usize __r_i = idx + 1; __r_i < da_len(list); __r_i++) {                                   \
      da_get(list, __r_i - 1) = da_get(list, __r_i);                                               \
    }                                                                                              \
    da_len(list)--;                                                                                \
  } while (0)

///////////////////////////////////////////////////////////////////////////////

#define da_map(src, dest, map)                                                                     \
  do {                                                                                             \
    da_reserve((dest), da_len(src));                                                               \
    for (usize __m_i = 0; __m_i < da_len(src); __m_i++) {                                          \
      da_get(dest, __m_i) = map(da_get(src, __m_i));                                               \
    }                                                                                              \
    da_len(dest) = da_len(src);                                                                    \
  } while (0)

#define da_map_ctx(src, dest, map, ctx)                                                            \
  do {                                                                                             \
    da_reserve((dest), da_len(src));                                                               \
    for (usize __m_i = 0; __m_i < da_len(src); __m_i++) {                                          \
      da_get(dest, __m_i) = map(ctx, da_get(src, __m_i));                                          \
    }                                                                                              \
    da_len(dest) = da_len(src);                                                                    \
  } while (0)

#define da_filter(src, dest, filter)                                                               \
  do {                                                                                             \
    da_reserve((dest), da_len(src));                                                               \
    usize __f_count = 0;                                                                           \
    for (usize __f_i = 0; __f_i < da_len(src); __f_i++) {                                          \
      if (filter(da_get(src, __f_i))) {                                                            \
        da_get(dest, __f_count++) = da_get(src, __f_i);                                            \
      }                                                                                            \
    }                                                                                              \
    da_len(dest) = __f_count;                                                                      \
  } while (0)

#define da_filter_ctx(src, dest, filter, ctx)                                                      \
  do {                                                                                             \
    da_reserve((dest), da_len(src));                                                               \
    usize __f_count = 0;                                                                           \
    for (usize __f_i = 0; __f_i < da_len(src); __f_i++) {                                          \
      if (filter((ctx), da_get(src, __f_i))) {                                                     \
        da_get(dest, __f_count++) = da_get(src, __f_i);                                            \
      }                                                                                            \
    }                                                                                              \
    da_len(dest) = __f_count;                                                                      \
  } while (0)

#define da_sort(src, sort) qsort(&da_get(src, 0), da_len(src), sizeof((src)->items[0]), sort)

#define da_reverse(list)                                                                           \
  do {                                                                                             \
    da_reserve((list), 1);                                                                         \
    for (usize __r_i = 0; __r_i < (list)->len - __r_i - 1; __r_i++) {                              \
      da_get(list, da_len(list)) = da_get(list, __r_i);                                            \
      da_get(list, __r_i) = da_get(list, da_len(list) - __r_i - 1);                                \
      da_get(list, da_len(list) - __r_i - 1) = da_get(list, da_len(list));                         \
    }                                                                                              \
  } while (0)

#define da_for_each(T, iter, da) for (T iter = &da_first(da); iter <= &da_last(da); iter++)

///////////////////////////////////////////////////////////////////////////////

#endif /* !__CEBUS_DA_H__ */
