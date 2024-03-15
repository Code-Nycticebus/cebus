/* DOCUMENTATION

All these functions are defined for these types: `u8`, `i8`, `u16`, `i16`, `u32`, `i32`, `u64`, `i64`, `usize`.

## Bit Operations

- `T_reverse_bits(T value)`: Reverses the bits in `value`.
- `T_leading_ones(T value)`: Counts the number of leading ones in `value`.
- `T_trailing_ones(T value)`: Counts the number of trailing ones in `value`.
- `T_leading_zeros(T value)`: Counts the number of leading zeros in `value`.
- `T_trailing_zeros(T value)`: Counts the number of trailing zeros in `value`.
- `T_count_zeros(T value)`: Counts the number of zeros in `value`.
- `T_count_ones(T value)`: Counts the number of ones in `value`.

## Byte Order Operations

- `T_swap_bytes(T value)`: Swaps the byte order of `value`.
- `T_to_be(T value)`: Converts `value` to big endian format.
- `T_from_be(T value)`: Converts `value` from big endian format.
- `T_to_le(T value)`: Converts `value` to little endian format.
- `T_from_le(T value)`: Converts `value` from little endian format.
- `T_to_ne_bytes(T value)`: Converts `value` to native endian format.

## Math Operations

Basic math operations including max, min, and clamp are provided.

- `T_max(T a, T b)`: Returns the maximum of `a` and `b`.
- `T_min(T a, T b)`: Returns the minimum of `a` and `b`.
- `T_clamp(T min, T max, T value)`: Clamps `value` between `min` and `max`.

## Utilities

- `T_hash(T value)`: Generates a hash for `value`.
- `T_swap(T *v1, T *v2)`: Swaps the values of `v1` and `v2`.
- `T_compare_lt(T a, T b)`: Compares `a` and `b` for less than.
- `T_compare_gt(T a, T b)`: Compares `a` and `b` for greater than.
- `T_compare_qsort(CmpOrdering ordering)`: Provides a comparison function
suitable for `qsort`.

*/

#ifndef __CLIB_INTEGERS_H__
#define __CLIB_INTEGERS_H__

#include "clib/core/arena.h"
#include "clib/core/defines.h"

#define INTEGER_DECL(T)                                                        \
  /* BIT OPERATIONS */                                                         \
  CONST_FN T T##_reverse_bits(T value);                                        \
  CONST_FN usize T##_leading_ones(T value);                                    \
  CONST_FN usize T##_trailing_ones(T value);                                   \
  CONST_FN usize T##_leading_zeros(T value);                                   \
  CONST_FN usize T##_trailing_zeros(T value);                                  \
  CONST_FN usize T##_count_zeros(T value);                                     \
  CONST_FN usize T##_count_ones(T value);                                      \
  /* BIT OPERATIONS END */                                                     \
                                                                               \
  /* BYTE ORDER OPERATIONS */                                                  \
  CONST_FN T T##_swap_bytes(T value);                                          \
  /*  BIG ENDIAN OPERATIONS */                                                 \
  CONST_FN T T##_to_be(T value);                                               \
  CONST_FN T T##_from_be(T value);                                             \
  T T##_from_be_bytes(Bytes bytes);                                            \
  Bytes T##_to_be_bytes(T value, Arena *arena);                                \
  /*  LITTLE ENDIAN OPERATIONS */                                              \
  CONST_FN T T##_to_le(T value);                                               \
  CONST_FN T T##_from_le(T value);                                             \
  T T##_from_le_bytes(Bytes bytes);                                            \
  Bytes T##_to_le_bytes(T value, Arena *arena);                                \
  /*  NATIVE ENDIAN OPERATIONS */                                              \
  T T##_from_ne_bytes(Bytes bytes);                                            \
  Bytes T##_to_ne_bytes(T value, Arena *arena);                                \
  /* BYTE ORDER OPERATIONS END */                                              \
                                                                               \
  /* MATH OPERATIONS */                                                        \
  CONST_FN T T##_max(T a, T b);                                                \
  CONST_FN T T##_min(T a, T b);                                                \
  CONST_FN T T##_clamp(T min, T max, T value);                                 \
  /* MATH OPERATIONS END */                                                    \
                                                                               \
  /* UTILS */                                                                  \
  CONST_FN u64 T##_hash(T value);                                              \
  void T##_swap(T *v1, T *v2);                                                 \
  CONST_FN CmpOrdering T##_compare_lt(T a, T b);                               \
  CONST_FN CmpOrdering T##_compare_gt(T a, T b);                               \
  CONST_FN CompareFn T##_compare_qsort(CmpOrdering ordering);                  \
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

#undef INTEGER_DECL

#endif /* !__CLIB_INTEGERS_H__ */
