#include "clib/asserts.h"
#include "datatypes/bytes.h"
#include "datatypes/str.h"

static void test_bytes(void) {
  Arena arena = {0};
  Bytes b = BYTES(0x02, 0xFF, 0xAA, 0xBB, 0x41, 0x41, 0x41);
  clib_assert(b.data[0] == 0x02, "Bytes are not initialized correctly");
  clib_assert(b.data[1] == 0xff, "Bytes are not initialized correctly");
  clib_assert(b.data[2] == 0xaa, "Bytes are not initialized correctly");
  clib_assert(b.data[3] == 0xbb, "Bytes are not initialized correctly");
  arena_free(&arena);
}

static void test_bytes_str(void) {
  Bytes b = BYTES_STR("ABC");
  clib_assert(b.data[0] == 0x41, "Bytes are not initialized correctly");
  clib_assert(b.data[1] == 0x42, "Bytes are not initialized correctly");
  clib_assert(b.data[2] == 0x43, "Bytes are not initialized correctly");
}

static void test_to_hex(void) {
  Arena arena = {0};
  Bytes b = BYTES(0x41, 0x42, 0x43);
  Str s = bytes_hex(b, &arena);
  clib_assert(str_eq(s, STR("| 3 | 41 42 43                | ABC     ")),
              "String conversion was not correct: '" STR_FMT "'", STR_ARG(s));

  Bytes b2 = BYTES(0x02, 0xFF, 0xAA, 0xBB, 0x41, 0x41, 0x41);
  Str s2 = bytes_hex(b2, &arena);
  clib_assert(str_eq(s2, STR("| 7 | 02 ff aa bb 41 41 41    | ....AAA ")),
              "String conversion was not correct: \n'" STR_FMT "'",
              STR_ARG(s2));

  Bytes b3 = BYTES(0xaa, 0xbb, 0xcc, 0xdd, 0x41, 0x41, 0x41, 0x41, 0x42, 0x42,
                   0x42, 0x42);
  Str s3 = bytes_hex(b3, &arena);
  clib_assert(str_eq(s3, STR("|  8 | aa bb cc dd 41 41 41 41 | ....AAAA\n"
                             "| 12 | 42 42 42 42             | BBBB    ")),
              "String conversion was not correct: \n" STR_FMT "", STR_ARG(s3));

  arena_free(&arena);
}

static void test_bytes_cmp(void) {
  Bytes b1 = BYTES(0xFF);
  clib_assert(bytes_eq(b1, BYTES(0xFF)) == true,
              "Bytes not compared correctly");
  clib_assert(bytes_eq(b1, BYTES(0x0F)) == false,
              "Bytes not compared correctly");
}

static void test_bytes_slice(void) {
  Bytes b = BYTES_STR("ABC");
  Bytes slice = bytes_slice(b, 1, 3);
  clib_assert(bytes_eq(slice, BYTES_STR("BC")),
              "Bytes were not sliced correctly");

  Bytes invalid = bytes_slice(b, b.size, 2);
  clib_assert(bytes_eq(invalid, BYTES_STR("")), "Bytes were not invalidated");

  Bytes invalid2 = bytes_slice(b, 0, b.size + 1);
  clib_assert(bytes_eq(invalid2, BYTES_STR("")), "Bytes were not invalidated");
}

static void test_bytes_take(void) {
  Bytes bytes = BYTES(0x12, 0x34, 0x56, 0x78);
  Bytes take = bytes_take(&bytes, 3);
  clib_assert(bytes_eq(bytes, BYTES(0x78)), "Did not take the correct bytes");
  clib_assert(bytes_eq(take, BYTES(0x12, 0x34, 0x56)),
              "Did not take the correct bytes");
  Bytes rest = bytes_take(&bytes, 3);
  clib_assert(bytes_eq(rest, BYTES(0x78)), "Did not take the correct bytes");

  Bytes invalid = bytes_take(&bytes, 3);
  clib_assert(invalid.size == 0, "Did not take the correct amount of bytes");
}

int main(void) {
  test_bytes();
  test_bytes_str();
  test_to_hex();
  test_bytes_cmp();
  test_bytes_slice();
  test_bytes_take();
}
