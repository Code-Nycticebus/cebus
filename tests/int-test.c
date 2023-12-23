#include "datatypes/integers.h"

#include "clib/asserts.h"

void test_u8_leading_bits(void) {
  const u8 b = 0xf0;
  clib_assert(u8_leading_ones(b) == 4, "Did not count correctly");
  const u8 b2 = 0x0f;
  clib_assert(u8_leading_zeros(b2) == 4, "Did not count correctly");
}

void test_u8__swaping_bits(void) {
  const u8 b = 0x80;
  clib_assert(u8_reverse_bits(b) == 0x01, "Did not reverse correctly");
  const u8 b2 = 0x01;
  clib_assert(u8_reverse_bits(b2) == 0x80, "Did not reverse correctly");
}

int main(void) {
  test_u8_leading_bits();
  test_u8__swaping_bits();
}
