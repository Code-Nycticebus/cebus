#include "types/char.h"

#include "core/asserts.h"

static void test_char_test(void) {
  clib_assert(c_is_alpha('a') == true, "Should be alpha");
  clib_assert(c_is_alpha('A') == true, "Should be alpha");
  clib_assert(c_is_alpha('1') == false, "Should not be alpha");

  clib_assert(c_is_digit('1') == true, "Should be digit");
  clib_assert(c_is_digit('a') == false, "Should not be digit");

  clib_assert(c_is_xdigit('1') == true, "Should be xdigit");
  clib_assert(c_is_xdigit('a') == true, "Should be xdigit");
  clib_assert(c_is_xdigit('A') == true, "Should be xdigit");
  clib_assert(c_is_xdigit('z') == false, "Should not be xdigit");
}

static void test_char_convertion(void) {
  clib_assert(c_to_upper('a') == 'A', "should be 'A'");
  clib_assert(c_to_lower('A') == 'a', "should be 'a'");

  clib_assert(c_to_digit('2') == 2, "should be 2");
  clib_assert(c_to_digit('2') == 2, "should be 2");

  clib_assert(c_to_xdigit('2') == 2, "should be 2");
  clib_assert(c_to_xdigit('a') == 10, "should be 10");
  clib_assert(c_to_xdigit('A') == 10, "should be 10");
}

int main(void) {
  test_char_test();
  test_char_convertion();
}
