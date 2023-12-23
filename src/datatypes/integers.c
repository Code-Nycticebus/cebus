#include "integers.h"

#include "clib/platform.h"
#include "datatypes/str.h"

u8 u8_reverse_bits(u8 value) {
  u8 reversed = 0;
  for (usize i = 0; i < U8_BITS; i++) {
    reversed = reversed << 1;
    if (value & 1) {
      reversed = reversed | 1;
    }
    value = value >> 1;
  }
  return reversed;
}

usize u8_leading_ones(u8 value) {
  usize count = 0;
  for (usize i = 0; i < U8_BITS; i++) {
    if (!(value & (0x80 >> i))) { // NOLINT
      break;
    }
    count++;
  }
  return count;
}

usize u8_leading_zeros(u8 value) {
  usize count = 0;
  for (usize i = 0; i < U8_BITS; i++) {
    if (value & (0x80 >> i)) { // NOLINT
      break;
    }
    count++;
  }
  return count;
}

u8 u8_from_be(u8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_LITTLE
  return u8_reverse_bits(value);
#else
  return value;
#endif
}

u8 u8_from_le(u8 value) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  return u8_reverse_bits(value);
#else
  return value;
#endif
}

Str u8_to_str(u8 value, Arena *arena) { return str_u64(arena, value); }
