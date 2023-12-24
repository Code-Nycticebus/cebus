#include "integers.h"

#include "clib/platform.h"
#include "datatypes/str.h"

#define BITS_REVERSE(T, value, BITS)                                           \
  do {                                                                         \
    T reversed = 0;                                                            \
    for (usize i = 0; i < BITS; i++) {                                         \
      reversed = reversed << 1;                                                \
      if (value & 1) {                                                         \
        reversed = reversed | 1;                                               \
      }                                                                        \
      value = value >> 1;                                                      \
    }                                                                          \
    return reversed;                                                           \
  } while (0)

#define BITS_LEADING_ONES(value, BITS)                                         \
  do {                                                                         \
    usize count = 0;                                                           \
    for (usize i = 0; i < U8_BITS; i++) {                                      \
      if (!(value & (0x80 >> i))) {                                            \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  } while (0)

#define BITS_LEADING_ZEROS(value, BITS)                                        \
  do {                                                                         \
    usize count = 0;                                                           \
    for (usize i = 0; i < BITS; i++) {                                         \
      if (value & (0x80 >> i)) {                                               \
        break;                                                                 \
      }                                                                        \
      count++;                                                                 \
    }                                                                          \
    return count;                                                              \
  } while (0)

u8 u8_reverse_bits(u8 value) { BITS_REVERSE(u8, value, U8_BITS); }
usize u8_leading_ones(u8 value) { BITS_LEADING_ONES(value, U8_BITS); }
usize u8_leading_zeros(u8 value) { BITS_LEADING_ZEROS(value, U8_BITS); }

u8 u8_to_be(u8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u8_reverse_bits(value);
#else
  return value;
#endif
}

u8 u8_to_le(u8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u8_reverse_bits(value);
#else
  return value;
#endif
}

Str u8_to_str(u8 value, Arena *arena) { return str_u64(arena, value); }
