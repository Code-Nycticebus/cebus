#include "type/char.h"

#include "core/assert.h"

static void test_char_test(void) {
  clib_assert(c_is_alnum('a') == true, "Did not test correctly");
  clib_assert(c_is_alnum('A') == true, "Did not test correctly");
  clib_assert(c_is_alnum('1') == true, "Did not test correctly");
  clib_assert(c_is_alnum(' ') == false, "Did not test correctly");

  clib_assert(c_is_alpha('a') == true, "Did not test correctly");
  clib_assert(c_is_alpha('A') == true, "Did not test correctly");
  clib_assert(c_is_alpha('1') == false, "Did not test correctly");

  clib_assert(c_is_lower('a') == true, "Did not test correctly");
  clib_assert(c_is_lower('A') == false, "Did not test correctly");

  clib_assert(c_is_upper('A') == true, "Did not test correctly");
  clib_assert(c_is_upper('a') == false, "Did not test correctly");

  clib_assert(c_is_space(' ') == true, "Did not test correctly");
  clib_assert(c_is_space('a') == false, "Did not test correctly");

  clib_assert(c_is_cntrl('\0') == true, "Did not test correctly");
  clib_assert(c_is_cntrl('\n') == true, "Did not test correctly");
  clib_assert(c_is_cntrl('a') == false, "Did not test correctly");

  clib_assert(c_is_print('a') == true, "Did not test correctly");
  clib_assert(c_is_print(' ') == true, "Did not test correctly");
  clib_assert(c_is_print('\0') == false, "Did not test correctly");

  clib_assert(c_is_graph('a') == true, "Did not test correctly");
  clib_assert(c_is_graph(' ') == false, "Did not test correctly");

  clib_assert(c_is_blank(' ') == true, "Did not test correctly");
  clib_assert(c_is_blank('\t') == true, "Did not test correctly");
  clib_assert(c_is_blank('\n') == false, "Did not test correctly");

  clib_assert(c_is_punct('.') == true, "Did not test correctly");
  clib_assert(c_is_punct('[') == true, "Did not test correctly");
  clib_assert(c_is_punct(']') == true, "Did not test correctly");
  clib_assert(c_is_punct('a') == false, "Did not test correctly");

  clib_assert(c_is_digit('1') == true, "Did not test correctly");
  clib_assert(c_is_digit('a') == false, "Did not test correctly");

  clib_assert(c_is_xdigit('1') == true, "Did not test correctly");
  clib_assert(c_is_xdigit('a') == true, "Did not test correctly");
  clib_assert(c_is_xdigit('A') == true, "Did not test correctly");
  clib_assert(c_is_xdigit('z') == false, "Did not test correctly");
}

static void test_char_convertion(void) {
  clib_assert(c_to_upper('a') == 'A', "should be 'A'");
  clib_assert(c_to_lower('A') == 'a', "should be 'a'");

  clib_assert(c_to_u8('2') == 2, "should be 2");
  clib_assert(c_u8_to_c(2) == '2', "should be '2'");

  clib_assert(c_hex_to_u8('2') == 2, "should be 2");
  clib_assert(c_hex_to_u8('a') == 10, "should be 10");
  clib_assert(c_hex_to_u8('A') == 10, "should be 10");

  clib_assert(c_u8_to_hex(2) == '2', "should be '2'");
  clib_assert(c_u8_to_hex(10) == 'a', "should be 'a'");
  clib_assert(c_u8_to_HEX(10) == 'A', "should be 'A'");
}

int main(void) {
  test_char_test();
  test_char_convertion();
}
