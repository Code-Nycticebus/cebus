#include "cebus/type/utf8.h"

#include "../test.h"
#include "cebus/core/debug.h"

#include <locale.h>
#include <string.h>

TEST(decode) {
  Bytes bytes = BYTES(        //
      0xF0, 0x9F, 0x8E, 0x89, // 🎉
      0xE2, 0x9C, 0x85,       // ✅
      0xF0, 0x9F, 0x98, 0x81, // 😁
      0x43,                   // C
      0x41                    // A
  );
  Utf8 s3 = utf8_decode(bytes, ErrPanic);
  cebus_assert(memcmp(s3.data, "🎉✅😁CA", s3.size) == 0, "Was not decoded correctly");

  Bytes garbage = BYTES(0xF0, 0x01);
  Error error = {0};
  utf8_decode(garbage, &error);
  cebus_assert(error.failure == true, "This should be a invalid utf-8");
  error_context(&error, { error_except(); });
}

TEST(creation) {
  Utf8 s = UTF8("🎉✅😁 Hellö  Wörld 💩");
  cebus_assert(memcmp(s.data, "🎉✅😁 Hellö  Wörld 💩", s.size) == 0, "Was not created correctly");
}

TEST(cmp) {
  Utf8 s1 = UTF8("🎉✅😁CA");
  Utf8 s2 = UTF8("🎉✅😁CA");
  Utf8 s3 = UTF8("🎉✅✅CA");
  cebus_assert(utf8_eq(s1, s2) == true, "Strings should be equal");
  cebus_assert(utf8_eq(s1, s3) == false, "Strings should not be equal");

  cebus_assert(utf8_starts_with(s1, UTF8("🎉✅")) == true, "Did not handle prefix correctly");
  cebus_assert(utf8_starts_with(s1, UTF8("🎉✅🎉✅🎉✅🎉✅")) == false,
               "Should detect that prefix is bigger");
  cebus_assert(utf8_starts_with(s1, UTF8("✅")) == false, "Should not start with ✅");

  cebus_assert(utf8_ends_with(s1, UTF8("😁CA")) == true, "Did not detect suffix");
  cebus_assert(utf8_ends_with(s1, UTF8("😁CA😁CA")) == false,
               "Should detect that suffix is bigger");
  cebus_assert(utf8_ends_with(s1, UTF8("😁")) == false, "Should not end with 😁");
}

TEST(copy) {
  Utf8 s = UTF8("🎉✅😁 Hellö  Wörld 💩");
  Utf8 s2 = utf8_copy(s, arena);
  cebus_assert(utf8_eq(s, s2) == true, "Utf8 strings not equal");
  cebus_assert(memcmp(s2.data, "🎉✅😁 Hellö  Wörld 💩", s.size) == 0, "Was not created correctly");
}

TEST(append) {

  Utf8 res = utf8_append(UTF8("🎉✅😁"), UTF8(" 💩"), arena);
  cebus_assert(utf8_eq(res, UTF8("🎉✅😁 💩")), "Did not append correctly");

  Utf8 res1 = utf8_prepend(UTF8("🎉✅😁"), UTF8("💩 "), arena);
  cebus_assert(utf8_eq(res1, UTF8("💩 🎉✅😁")), "Did not prepend correctly");
}

TEST(join) {

  Utf8 res = utf8_join(UTF8(" "), 3, (Utf8[3]){UTF8("🎉"), UTF8("✅"), UTF8("🎉")}, arena);

  cebus_assert(utf8_eq(res, UTF8("🎉 ✅ 🎉")), "String was not joined correctly");
}

TEST(case_transform) {

  Utf8 f = UTF8("Hello, 🌎");

  Utf8 upper = utf8_upper(f, arena);
  cebus_assert(utf8_eq(upper, UTF8("HELLO, 🌎")), "Did not transform correctly: " UTF8_FMT,
               UTF8_ARG(upper));

  Utf8 lower = utf8_lower(f, arena);
  cebus_assert(utf8_eq(lower, UTF8("hello, 🌎")), "Did not transform correctly: " UTF8_FMT,
               UTF8_ARG(upper));
}

TEST(next) {
  Utf8 test = UTF8("🎉✅😁CA");
  Utf8 one = utf8_next(&test);
  cebus_assert(utf8_eq(one, UTF8("🎉")), "Utf-8 was not correctly choped!");
  one = utf8_next(&test);
  cebus_assert(utf8_eq(one, UTF8("✅")), "Utf-8 was not correctly choped!");
  one = utf8_next(&test);
  cebus_assert(utf8_eq(one, UTF8("😁")), "Utf-8 was not correctly choped!");

  bool ret = utf8_try_next(&test, &one);
  cebus_assert(utf8_eq(one, UTF8("C")), "Utf-8 was not correctly choped!");
  cebus_assert(ret, "Utf-8 was not correctly choped!");
  ret = utf8_try_next(&test, &one);
  cebus_assert(utf8_eq(one, UTF8("A")), "Utf-8 was not correctly choped!");
  cebus_assert(ret, "Utf-8 was not correctly choped!");

  ret = utf8_try_next(&test, &one);
  cebus_assert(ret == false, "Utf-8 was not correctly choped!");
}

int main(void) {
  setlocale(LC_ALL, "");

  Arena arena = {0};

  TEST_RUN(decode, &arena);
  TEST_RUN(creation, &arena);
  TEST_RUN(cmp, &arena);
  TEST_RUN(copy, &arena);
  TEST_RUN(append, &arena);
  TEST_RUN(join, &arena);
  TEST_RUN(case_transform, &arena);

  TEST_RUN(next, &arena);

  arena_free(&arena);
}
