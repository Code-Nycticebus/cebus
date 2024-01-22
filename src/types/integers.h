#ifndef __CLIB_INTEGERS_H__
#define __CLIB_INTEGERS_H__

/* DOCUMENTATION
## Usage
Just use the functions.
*/

#include "core/arena.h"
#include "core/defines.h"

#define INTEGER_DECL(T)                                                        \
  /* BIT OPERATIONS */                                                         \
  T const_fn T##_reverse_bits(T value);                                        \
  usize const_fn T##_leading_ones(T value);                                    \
  usize const_fn T##_trailing_ones(T value);                                   \
  usize const_fn T##_leading_zeros(T value);                                   \
  usize const_fn T##_trailing_zeros(T value);                                  \
  usize const_fn T##_count_zeros(T value);                                     \
  usize const_fn T##_count_ones(T value);                                      \
  /* BIT OPERATIONS END */                                                     \
                                                                               \
  /* BYTE ORDER OPERATIONS */                                                  \
  T const_fn T##_swap_bytes(T value);                                          \
  /*  BIG ENDIAN OPERATIONS */                                                 \
  T const_fn T##_to_be(T value);                                               \
  T const_fn T##_from_be(T value);                                             \
  T T##_from_be_bytes(Bytes bytes);                                            \
  Bytes T##_to_be_bytes(T value, Arena *arena);                                \
  /*  LITTLE ENDIAN OPERATIONS */                                              \
  T const_fn T##_to_le(T value);                                               \
  T const_fn T##_from_le(T value);                                             \
  T T##_from_le_bytes(Bytes bytes);                                            \
  Bytes T##_to_le_bytes(T value, Arena *arena);                                \
  /*  NATIVE ENDIAN OPERATIONS */                                              \
  T T##_from_ne_bytes(Bytes bytes);                                            \
  Bytes T##_to_ne_bytes(T value, Arena *arena);                                \
  /* BYTE ORDER OPERATIONS END */                                              \
                                                                               \
  /* MATH OPERATIONS */                                                        \
  T const_fn T##_max(T a, T b);                                                \
  T const_fn T##_min(T a, T b);                                                \
  T const_fn T##_clamp(T min, T max, T value);                                 \
  /* MATH OPERATIONS END */                                                    \
                                                                               \
  /* UTILS */                                                                  \
  u64 const_fn T##_hash(T value);                                              \
  void T##_swap(T *v1, T *v2);                                                 \
  CmpOrdering const_fn T##_compare_lt(T a, T b);                               \
  CmpOrdering const_fn T##_compare_gt(T a, T b);                               \
  CompareFn const_fn T##_compare_qsort(CmpOrdering ordering);                  \
  /* UTILS END */

INTEGER_DECL(u8)
INTEGER_DECL(i8)
INTEGER_DECL(u16)
INTEGER_DECL(i16)
INTEGER_DECL(u32)
INTEGER_DECL(i32)
INTEGER_DECL(u64)
INTEGER_DECL(i64)
INTEGER_DECL(usize)

#endif /* !__CLIB_INTEGERS_H__ */
