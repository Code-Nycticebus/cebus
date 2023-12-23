#include "integers.h"

u8 u8_reverse_bits(u8 value) {
  return ((value & 0xf0) >> 4) | ((value & 0x0f) << 4); // NOLINT
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
