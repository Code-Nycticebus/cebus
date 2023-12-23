#include "datatypes/integers.h"

#include <stdio.h>

#include "clib/asserts.h"

void test_leading_bits(void) {
  const uint8_t b = 0xf0;
  clib_assert(u8_leading_ones(b) == 4, "Did not count correctly");

  const uint8_t b2 = 0x0f;
  clib_assert(u8_leading_zeros(b2) == 4, "Did not count correctly");

  const uint8_t b3 = 0xc0;
  clib_assert(u8_leading_ones(b3) == 2, "Did not count correctly");

  const uint8_t b4 = 0x3c;
  clib_assert(u8_leading_zeros(b4) == 2, "Did not count correctly");
}

int main(void) {
  test_leading_bits();
  u8 b = u8_reverse_bits(0x81);
  printf("%x\n", b);
}
