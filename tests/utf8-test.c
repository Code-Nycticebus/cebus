#include <locale.h>

#include "clib/asserts.h"
#include "datatypes/bytes.h"
#include "datatypes/utf8.h"

#include <string.h>

static void test_decode(void) {
  Bytes bytes = BYTES(        //
      0xF0, 0x9F, 0x8E, 0x89, // 🎉
      0xE2, 0x9C, 0x85,       // ✅
      0xF0, 0x9F, 0x98, 0x81, // 😁
      0x43,                   // C
      0x41                    // A
  );
  Utf8 s3 = utf8_decode(bytes);
  clib_assert(memcmp(s3.data, "🎉✅😁CA", s3.size) == 0,
              "Was not decoded correctly");

  Bytes garbage = BYTES(0xF0, 0x01);
  Utf8 res = {0};
  bool ret = utf8_try_decode(garbage, &res);
  clib_assert(ret == false, "This should not be a valid utf-8 string");
}

static void test_creation(void) {
  Utf8 s = UTF8("🎉✅😁 Hellö  Wörld 💩");
  clib_assert(memcmp(s.data, "🎉✅😁 Hellö  Wörld 💩", s.size) == 0,
              "Was not created correctly");
}

static void test_copy(void) {
  Arena arena = {0};
  Utf8 s = UTF8("🎉✅😁 Hellö  Wörld 💩");
  Utf8 s2 = utf8_copy(s, &arena);
  clib_assert(utf8_eq(s, s2) == true, "Utf8 strings not equal");
  clib_assert(memcmp(s2.data, "🎉✅😁 Hellö  Wörld 💩", s.size) == 0,
              "Was not created correctly");
  arena_free(&arena);
}

static void test_concat(void) {
  Arena arena = {0};

  Utf8 res = utf8_concat(UTF8("🎉✅😁"), UTF8(" 💩"), &arena);
  clib_assert(utf8_eq(res, UTF8("🎉✅😁 💩")), "Did not concat correctly");

  arena_free(&arena);
}

static void test_join(void) {
  Arena arena = {0};

  Utf8 res = utf8_join(UTF8(" "), 3,
                       (Utf8[]){UTF8("🎉"), UTF8("✅"), UTF8("🎉")}, &arena);

  clib_assert(utf8_eq(res, UTF8("🎉 ✅ 🎉")),
              "String was not joined correctly");

  arena_free(&arena);
}

static void test_next(void) {
  Utf8 test = UTF8("🎉✅😁CA");
  Utf8 one = utf8_next(&test);
  clib_assert(utf8_eq(one, UTF8("🎉")), "Utf-8 was not correctly choped!");
  one = utf8_next(&test);
  clib_assert(utf8_eq(one, UTF8("✅")), "Utf-8 was not correctly choped!");
  one = utf8_next(&test);
  clib_assert(utf8_eq(one, UTF8("😁")), "Utf-8 was not correctly choped!");

  bool ret = utf8_try_next(&test, &one);
  clib_assert(utf8_eq(one, UTF8("C")), "Utf-8 was not correctly choped!");
  clib_assert(ret, "Utf-8 was not correctly choped!");
  ret = utf8_try_next(&test, &one);
  clib_assert(utf8_eq(one, UTF8("A")), "Utf-8 was not correctly choped!");
  clib_assert(ret, "Utf-8 was not correctly choped!");

  ret = utf8_try_next(&test, &one);
  clib_assert(ret == false, "Utf-8 was not correctly choped!");
}

int main(void) {
  setlocale(LC_ALL, "");

  test_decode();
  test_creation();
  test_copy();
  test_concat();
  test_join();
  test_next();
}
