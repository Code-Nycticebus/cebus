#include "datatypes/integers.h"

#include "clib/asserts.h"

void test_u8_leading_bits(void) {
  clib_assert(u8_leading_ones(0xf0) == 4, "Did not count correctly");
  clib_assert(u8_leading_zeros(0x0f) == 4, "Did not count correctly");
  clib_assert(u8_trailing_ones(0x07) == 3, "Did not count correctly");
  clib_assert(u8_trailing_zeros(0x08) == 3, "Did not count correctly");
}

void test_u8_swaping_bits(void) {
  clib_assert(u8_reverse_bits(0x80) == 0x01, "Did not reverse correctly");
  clib_assert(u8_reverse_bits(0x01) == 0x80, "Did not reverse correctly");
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
  test_u8_swaping_bits();
  test_u8_endian();
  test_u8_count_bits();
}
