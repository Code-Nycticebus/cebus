#ifndef __CLIB_INTEGERS_H__
#define __CLIB_INTEGERS_H__

#include "core/arena.h"
#include "core/defines.h"

#define INTEGER_DECL(T)                                                        \
  /*                                                                           \
   * Returns a new integer with its bits reversed.                             \
   */                                                                          \
  T T##_reverse_bits(T value);                                                 \
                                                                               \
  /*                                                                           \
   * Returns a new integer with its bytes reversed.                            \
   */                                                                          \
  T T##_swap_bytes(T value);                                                   \
                                                                               \
  /*                                                                           \
   * Returns the count of leading ones.                                        \
   */                                                                          \
  usize T##_leading_ones(T value);                                             \
                                                                               \
  /*                                                                           \
   * Returns the count of trailing ones.                                       \
   */                                                                          \
  usize T##_trailing_ones(T value);                                            \
                                                                               \
  /*                                                                           \
   * Returns the count of leading zeros.                                       \
   */                                                                          \
  usize T##_leading_zeros(T value);                                            \
                                                                               \
  /*                                                                           \
   * Returns the count of trailing zeros.                                      \
   */                                                                          \
  usize T##_trailing_zeros(T value);                                           \
                                                                               \
  /*                                                                           \
   *  Counts bits that are set to one.                                         \
   */                                                                          \
  usize T##_count_zeros(T value);                                              \
                                                                               \
  /*                                                                           \
   *  Counts bits that are set to one.                                         \
   */                                                                          \
  usize T##_count_ones(T value);                                               \
                                                                               \
  /*                                                                           \
   * Returns new integer in big-endian byte order from native byte order.      \
   */                                                                          \
  T T##_to_be(T value);                                                        \
                                                                               \
  /*                                                                           \
   * Returns new integer in native byte order from big-endian byte order.      \
   */                                                                          \
  T T##_from_be(T value);                                                      \
                                                                               \
  /*                                                                           \
   * Returns new integer in native byte order from big-endian byte order.      \
   */                                                                          \
  T T##_from_be_bytes(Bytes bytes);                                            \
                                                                               \
  /*                                                                           \
   * Returns Bytes in big-endian byte order from integer.                      \
   */                                                                          \
  Bytes T##_to_be_bytes(T value, Arena *arena);                                \
                                                                               \
  /*                                                                           \
   * Returns new integer in little-endian byte order from native byte order.   \
   */                                                                          \
  T T##_to_le(T value);                                                        \
                                                                               \
  /*                                                                           \
   * Returns new integer in native byte order from little-endian byte order.   \
   */                                                                          \
  T T##_from_le(T value);                                                      \
                                                                               \
  /*                                                                           \
   * Returns new integer in native byte order from little-endian byte order.   \
   */                                                                          \
  T T##_from_le_bytes(Bytes bytes);                                            \
                                                                               \
  /*                                                                           \
   * Returns Bytes in little-endian byte order from integer.                   \
   */                                                                          \
  Bytes T##_to_le_bytes(T value, Arena *arena);                                \
                                                                               \
  /*                                                                           \
   * Returns new integer in native byte order from native byte order.          \
   */                                                                          \
  T T##_from_ne_bytes(Bytes bytes);                                            \
                                                                               \
  /*                                                                           \
   * Returns Bytes in native byte order from integer.                          \
   */                                                                          \
  Bytes T##_to_ne_bytes(T value, Arena *arena);                                \
                                                                               \
  /*                                                                           \
   *  Returns the bigger out of 'a' and 'b'.                                   \
   */                                                                          \
  T T##_max(T a, T b);                                                         \
                                                                               \
  /*                                                                           \
   *  Returns the smaller out of 'a' and 'b'.                                  \
   */                                                                          \
  T T##_min(T a, T b);                                                         \
                                                                               \
  /*                                                                           \
   * Returns 'value' clamped to 'min' and 'max'.                               \
   */                                                                          \
  T T##_clamp(T min, T max, T value);                                          \
                                                                               \
  /*                                                                           \
   * Basic hash.                                                               \
   */                                                                          \
  u64 T##_hash(T value);                                                       \
                                                                               \
  /*                                                                           \
   * Swaps two integer variables.                                              \
   */                                                                          \
  void T##_swap(T *v1, T *v2);                                                 \
                                                                               \
  /*                                                                           \
   * Compares less than.                                                       \
   */                                                                          \
  CmpOrdering T##_compare_lt(T a, T b);                                        \
                                                                               \
  /*                                                                           \
   * Compares greater than.                                                    \
   */                                                                          \
  CmpOrdering T##_compare_gt(T a, T b);                                        \
                                                                               \
  /*                                                                           \
   * Returns a function that you can pass to qsort.                            \
   */                                                                          \
  CompareFn T##_compare_qsort(CmpOrdering ordering);

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
