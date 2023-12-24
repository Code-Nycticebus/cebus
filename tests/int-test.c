#include "datatypes/integers.h"

#include "clib/asserts.h"

void test_u8_leading_bits(void) {
  const u8 b = 0xf0;
  clib_assert(u8_leading_ones(b) == 4, "Did not count correctly");
  const u8 b2 = 0x0f;
  clib_assert(u8_leading_zeros(b2) == 4, "Did not count correctly");
}

void test_u8_trailing_bits(void) {
  const u8 b = 0x07;
  clib_assert(u8_trailing_ones(b) == 3, "Did not count correctly");
  const u8 b2 = 0x08;
  clib_assert(u8_trailing_zeros(b2) == 3, "Did not count correctly");
}

void test_u8_swaping_bits(void) {
  const u8 b = 0x80;
  clib_assert(u8_reverse_bits(b) == 0x01, "Did not reverse correctly");
  const u8 b2 = 0x01;
  clib_assert(u8_reverse_bits(b2) == 0x80, "Did not reverse correctly");
}

void test_u8_endian(void) {
#if CLIB_BYTE_ORDER == ENDIAN_BIG
  clib_assert(u8_to_le(0x80) == 0x80, "Bytes are somehow different");
  clib_assert(u8_to_be(0x80) == 0x80, "Bytes are somehow different")
#else
  clib_assert(u8_to_le(0x80) == 0x80, "Bytes are somehow different");
  clib_assert(u8_to_be(0x80) == 0x80, "Bytes are somehow different");
#endif
}

void test_u8_count_bits(void) {
  clib_assert(u8_count_ones(42) == 3, "Did count correctly");
  clib_assert(u8_count_zeros(42) == 5, "Did count correctly");
}

int main(void) {
  test_u8_leading_bits();
  test_u8_trailing_bits();
  test_u8_swaping_bits();
  test_u8_endian();
  test_u8_count_bits();
}
