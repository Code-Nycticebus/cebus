#include "datatypes/integers.h"

#include "clib/asserts.h"

void test_u8_leading_bits(void) {
  const u8 b = 0xf0;
  clib_assert(u8_leading_ones(b) == 4, "Did not count correctly");
  const u8 b2 = 0x0f;
  clib_assert(u8_leading_zeros(b2) == 4, "Did not count correctly");
}

void test_u8_swaping_bits(void) {
  const u8 b = 0x80;
  clib_assert(u8_reverse_bits(b) == 0x01, "Did not reverse correctly");
  const u8 b2 = 0x01;
  clib_assert(u8_reverse_bits(b2) == 0x80, "Did not reverse correctly");
}

void test_u8_endian(void) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  printf("BIG\n");
  clib_assert(u8_to_le(0x80) == 0x01, "Bits were not swapped");
  clib_assert(u8_to_be(0x80) == 0x80, "Bits are somehow different")
#else
  printf("LITTLE\n");
  clib_assert(u8_to_le(0x80) == 0x80, "Bits are somehow different");
  clib_assert(u8_to_be(0x80) == 0x01, "Bits are somehow different");
#endif
}

int main(void) {
  test_u8_leading_bits();
  test_u8_swaping_bits();
  test_u8_endian();
}
