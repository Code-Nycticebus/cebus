#ifndef __CLIB_INTEGERS_H__
#define __CLIB_INTEGERS_H__

#include "clib/arena.h"
#include "datatypes.h"

#define INTEGER_DECL(T)                                                        \
  T T##_reverse_bits(T value);                                                 \
  T T##_swap_bytes(T value);                                                   \
  usize T##_leading_ones(T value);                                             \
  usize T##_trailing_ones(T value);                                            \
  usize T##_leading_zeros(T value);                                            \
  usize T##_trailing_zeros(T value);                                           \
  usize T##_count_zeros(T value);                                              \
  usize T##_count_ones(T value);                                               \
  T T##_to_be(T value);                                                        \
  T T##_from_be(T value);                                                      \
  T T##_from_be_bytes(Bytes bytes);                                            \
  Bytes T##_to_be_bytes(T value, Arena *arena);                                \
  T T##_to_le(T value);                                                        \
  T T##_from_le(T value);                                                      \
  T T##_from_le_bytes(Bytes bytes);                                            \
  Bytes T##_to_le_bytes(T value, Arena *arena);                                \
  T T##_from_ne_bytes(Bytes bytes);                                            \
  Bytes T##_to_ne_bytes(T value, Arena *arena);

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