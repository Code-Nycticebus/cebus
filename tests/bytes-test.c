#include "clib/asserts.h"
#include "clib/bytes.h"

void test_bytes(void) {
  Arena arena = {0};
  Bytes b = BYTES(0x02, 0xFF, 0xAA, 0xBB);
  clib_assert(b.data[0] == 0x02, "Bytes are not initialized correctly");
  clib_assert(b.data[1] == 0xff, "Bytes are not initialized correctly");
  clib_assert(b.data[2] == 0xaa, "Bytes are not initialized correctly");
  clib_assert(b.data[3] == 0xbb, "Bytes are not initialized correctly");

  Str s = bytes_hex(b, &arena);
  clib_assert(str_eq(s, STR("02ffaabb")), "String conversion was not correct!");
  arena_free(&arena);
}

void test_bytes_str(void) {
  Arena arena = {0};
  Bytes b = BYTES_STR("ABC");
  clib_assert(b.data[0] == 0x41, "Bytes are not initialized correctly");
  clib_assert(b.data[1] == 0x42, "Bytes are not initialized correctly");
  clib_assert(b.data[2] == 0x43, "Bytes are not initialized correctly");

  Str s = bytes_hex(b, &arena);
  clib_assert(str_eq(s, STR("414243")), "String conversion was not correct!");
  arena_free(&arena);
}

int main(void) {
  test_bytes();
  test_bytes_str();
}
