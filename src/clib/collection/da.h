/* DOCUMENTATION
## Initialization

To start using the dynamic array, you first need to create an instance of
`Arena` for memory management and then initialize the dynamic array with the
specific type.

```c
Arena arena = {0};
DA(int) vec = {0};
da_init(&vec, &arena);
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

> :warning: These operations do not perform any bounds checks. So make sure your dynamic array has at least one element in it.

```c
int first = da_first(&vec);
int last = da_last(&vec);
int nth = da_get(&vec, 3);
int popped = da_pop(&vec);
```

## Manipulating the Array

- `da_clear`: Reset the length of the array to zero.
- `da_empty`: Use to check if the array has no elements.
- `da_init_list`: Initialize dynamic array with elements from a static array.
- `da_copy`: Duplicate the contents of one dynamic array into another.

## Resizing and Reserving Space

- `da_resize`: Adjust the capacity of the array to a specified size. Used for
preallocating space.
- `da_reserve`: Ensure there is enough space for additional elements. Used
before adding multiple elements.

## Extending and Mapping

- `da_extend`: Add multiple elements from another array or list.
- `da_map`: Transform elements of the array into another form and store them in
a destination array.
- `da_filter`: Filter dynamic array with a filter function and place it into a
destination.
- `da_filter_ctx`: Filter dynamic array with a filter function, that takes a
`void*` as a context, and place it into a destination.

## Sorting and Reversing

- `da_sort`: Sort the array using a comparison function.
- `da_sort_ctx`: Sort the array using a comparison function that also takes a
`void*` as a context.
- `da_reverse`: Reverse the order of elements in the array.
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
#define da_get(list, idx) (list)->items[idx]
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
