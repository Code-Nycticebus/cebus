#include "cebus/core/arena.h"
#include "cebus/core/defines.h"
#include "cebus/type/char.h"

#include "cebus/core/debug.h"
#include "cebus/type/string.h"

#include <stdlib.h>

static void test_compare(void) {
  Str s = STR("Hello, World");

  cebus_assert(str_eq(s, STR("Hello, World")) == true, "");
  cebus_assert(str_eq_ignorecase(s, STR("hello, world")) == true, "");
  cebus_assert(str_startswith(s, STR("Hello, ")) == true, "");
  cebus_assert(str_endswith(s, STR(", World")) == true, "");

  cebus_assert(str_contains(s, STR("Hell")) == true, "");
  cebus_assert(str_contains(s, STR("TEST")) == false, "");

  cebus_assert(str_contains(s, STR("H")) == true, "");
  cebus_assert(str_contains(s, STR("T")) == false, "");
  cebus_assert(str_contains(s, STR("")) == true, "");

  cebus_assert(str_includes(s, 'H') == true, "");
  cebus_assert(str_includes(s, 'e') == true, "");
  cebus_assert(str_includes(s, 'T') == false, "");

  cebus_assert(str_empty(s) == false, "string should not be empty");
  cebus_assert(str_empty(STR("")) == true, "string should be empty");
}

static void test_transform(void) {
  Arena arena = {0};
  Str s = STR("Hello, World");
  Str lower = str_lower(s, &arena);
  Str upper = str_upper(s, &arena);

  cebus_assert(str_eq(lower, STR("hello, world")), "");
  cebus_assert(str_eq(upper, STR("HELLO, WORLD")), "");

  arena_free(&arena);
}

static void test_copy(void) {
  Arena arena = {0};

  Str str = STR("Hello, World");
  Str full = str_copy(str, &arena);

  cebus_assert(str_eq(full, STR("Hello, World")), "");
  cebus_assert(str_getc(full, 0) == 'H', "");
  cebus_assert(str_getc(full, 1) == 'e', "");
  cebus_assert(str_getc(full, 7) == 'W', "");
  cebus_assert(str_getc(full, full.len) == '\0', "");

  arena_free(&arena);
}

static void test_append(void) {
  Arena arena = {0};
  Str append = str_append(STR("filename"), STR(".c"), &arena);
  cebus_assert(str_eq(append, STR("filename.c")), "did not append correctly");

  Str prepend = str_prepend(STR("filename.c"), STR("used_mark__"), &arena);
  cebus_assert(str_eq(prepend, STR("used_mark__filename.c")), "did not prepend correctly");

  Str wrap = str_wrap(STR("command here"), STR("'"), &arena);
  cebus_assert(str_eq(wrap, STR("'command here'")), "did not wrap correctly");

  arena_free(&arena);
}

static void test_trim(void) {
  Str dirty = STR("\t  Hello World  \n");
  Str trim_l = str_trim_left(dirty);
  cebus_assert(str_eq(trim_l, STR("Hello World  \n")), "");

  Str trim_r = str_trim_right(dirty);
  cebus_assert(str_eq(trim_r, STR("\t  Hello World")), "");

  Str trim = str_trim(dirty);
  cebus_assert(str_eq(trim, STR("Hello World")), "");
}

static void test_chop(void) {
  Str text = STR("Hello\nThis\nis\n\ntext");
  Str h = str_chop_by_delim(&text, '\n');
  cebus_assert(str_eq(h, STR("Hello")), STR_FMT, STR_ARG(h));

  Str rest = str_chop_by_predicate(&text, c_is_space);
  cebus_assert(str_eq(rest, STR("This")), STR_FMT, STR_ARG(rest));
  rest = str_chop_by_predicate(&text, c_is_space);
  cebus_assert(str_eq(rest, STR("is")), STR_FMT, STR_ARG(rest));
  rest = str_chop_by_predicate(&text, c_is_space);
  cebus_assert(str_eq(rest, STR("text")), STR_FMT, STR_ARG(rest));
}

static void test_try_chop(void) {
  Str text = STR("Hello\nThis\nis\n\ntext");
  Str h = {0};
  bool t1 = str_try_chop_by_delim(&text, '\n', &h);
  cebus_assert(t1 == true, "");
  cebus_assert(str_eq(h, STR("Hello")), STR_FMT, STR_ARG(h));

  Str rest = {0};
  bool t2 = str_try_chop_by_predicate(&text, c_is_space, &rest);
  cebus_assert(t2 == true, "");
  cebus_assert(str_eq(rest, STR("This")), STR_FMT, STR_ARG(rest));

  t2 = str_try_chop_by_predicate(&text, c_is_space, &rest);
  cebus_assert(t2 == true, "");
  cebus_assert(str_eq(rest, STR("is")), STR_FMT, STR_ARG(rest));

  t2 = str_try_chop_by_predicate(&text, c_is_space, &rest);
  cebus_assert(t2 == true, STR_FMT, STR_ARG(rest));
  cebus_assert(str_eq(rest, STR("text")), STR_FMT, STR_ARG(rest));

  bool t3 = str_try_chop_by_delim(&text, '\n', &rest);
  cebus_assert(t3 == false, STR_FMT, STR_ARG(rest));
}

static void test_chop_right(void) {
  Str text = STR("Hello\nThis is  text");
  Str t = str_chop_right_by_predicate(&text, c_is_space);
  Str rest = str_chop_right_by_delim(&text, '\n');
  Str rest2 = str_chop_right_by_delim(&text, '\n');
  Str rest3 = str_chop_right_by_delim(&text, '\n');

  cebus_assert(str_eq(t, STR("text")), "");
  cebus_assert(str_eq(rest, STR("This is")), "");
  cebus_assert(str_eq(rest2, STR("Hello")), "");
  cebus_assert(str_eq(rest3, STR("")), "");
}

static void test_number_converting(void) {
  Arena arena = {0};

  const u64 N = 64;
  Str number = str_format(&arena, "%" U64_FMT, N);
  cebus_assert(str_eq(number, STR("64")), "");

  Str n = str_append(number, STR(" bytes"), &arena);
  cebus_assert(str_eq(n, STR("64 bytes")), STR_FMT, STR_ARG(n));

  cebus_assert(str_u64(n) == 64, "");
  cebus_assert(str_chop_u64(&n) == 64, "");
  cebus_assert(n.len == 6, "%" USIZE_FMT, n.len);
  cebus_assert(str_eq(n, STR(" bytes")), STR_FMT, STR_ARG(n));

  Str f = STR("420.69");
  cebus_assert(str_f64(f) == 420.69, "");

  cebus_assert(str_chop_f64(&f) == 420.69, "");
  cebus_assert(f.len == 0, "%" USIZE_FMT, f.len);
  cebus_assert(str_eq(f, STR("")), STR_FMT, STR_ARG(f));

  arena_free(&arena);
}

static void test_find(void) {
  Str s = STR("Hello, World");
  cebus_assert(str_find(s, STR("Hello")) == 0, "");
  cebus_assert(str_find_last(s, STR("Hello")) == 0, "");
  cebus_assert(str_find(s, STR("World")) == 7, "");
  cebus_assert(str_find_last(s, STR("World")) == 7, "");
  cebus_assert(str_find(s, STR("TEST")) == STR_NOT_FOUND, "");
}

static void test_count(void) {
  Str s = STR("Hello, World");
  usize c = str_count(s, STR("o"));
  cebus_assert(c == 2, "");
  c = str_count(s, STR("TEST"));
  cebus_assert(c == 0, "");

  s = STR("--help");
  c = str_count(s, STR("-"));
  cebus_assert(c == 2, "");
}

static void test_replace(void) {
  Arena arena = {0};

  Str s = STR("Hello, World");
  Str goodbye = str_replace(s, STR("Hello"), STR("Goodbye"), &arena);
  Str all = str_replace(s, STR("World"), STR("All!"), &arena);

  cebus_assert(str_eq(s, STR("Hello, World")), "");
  cebus_assert(str_eq(goodbye, STR("Goodbye, World")), STR_FMT, STR_ARG(goodbye));
  cebus_assert(str_eq(all, STR("Hello, All!")), "");

  Str max_test = STR("test test test");
  Str result = str_replace(max_test, STR("test"), STR("result"), &arena);
  cebus_assert(str_eq(result, STR("result result result")), "");

  Str dash = STR("c-language");
  Str res = str_replace(dash, STR("-"), STR(""), &arena);
  cebus_assert(str_eq(res, STR("clanguage")), "");

  Str edge = STR("-language");
  Str res2 = str_replace(edge, STR("-"), STR(""), &arena);
  cebus_assert(str_eq(res2, STR("language")), "");

  arena_free(&arena);
}

static void test_substring(void) {
  Str s = STR("Hello, World");
  Str substring = str_substring(s, 0, 4);
  Str invalid1 = str_substring(s, 4, 2);
  Str invalid2 = str_substring(s, s.len, 2);

  cebus_assert(str_eq(substring, STR("Hell")), "");
  cebus_assert(str_eq(invalid1, STR("")), "");
  cebus_assert(str_eq(invalid2, STR("")), "");
}

static void test_join(void) {
  Arena arena = {0};
  Str res = str_join(STR(", "), 2, (Str[2]){STR("Hello"), STR("World")}, &arena);
  cebus_assert(str_eq(res, STR("Hello, World")), "");

  Str res2 = str_join_suffix(STR(", "), 2, (Str[2]){STR("Hello"), STR("World")}, &arena);
  cebus_assert(str_eq(res2, STR("Hello, World, ")), "");

  Str res3 = str_join_prefix(STR(", "), 2, (Str[2]){STR("Hello"), STR("World")}, &arena);
  cebus_assert(str_eq(res3, STR(", Hello, World")), "");

  Str res4 = str_join_wrap(STR(" "), STR("'"), 2, (Str[2]){STR("One"), STR("Two")}, &arena);
  cebus_assert(str_eq(res4, STR("'One' 'Two'")), STR_FMT, STR_ARG(res4));

  Str res5 = str_join(STR(" "), 0, NULL, &arena);
  cebus_assert(str_eq(res5, STR("")), STR_REPR, STR_ARG(res5));

  arena_free(&arena);
}

static void test_justify(void) {
  Arena arena = {0};
  const usize width = 10;
  Str center = str_center(STR("Hello"), width, ' ', &arena);
  cebus_assert(str_eq(center, STR("  Hello   ")), "");
  Str left = str_ljust(STR("Hello"), width, ' ', &arena);
  cebus_assert(str_eq(left, STR("Hello     ")), "");
  Str right = str_rjust(STR("Hello"), width, ' ', &arena);
  cebus_assert(str_eq(right, STR("     Hello")), "");
  arena_free(&arena);
}

static void test_cmp(void) {
  Str array[4] = {STR("Banana"), STR("Strawberry"), STR("Apple"), STR("Lemon")};
  const usize n = sizeof(array) / sizeof(array[0]);
  qsort(array, n, sizeof(array[0]), str_compare_qsort(CMP_GREATER));

  cebus_assert(str_eq(array[0], STR("Apple")), "");
  cebus_assert(str_eq(array[1], STR("Banana")), "");
  cebus_assert(str_eq(array[2], STR("Lemon")), "");
  cebus_assert(str_eq(array[3], STR("Strawberry")), "");
}

static void test_repeat(void) {
  Arena arena = {0};
  Str tf_fleet = str_repeat(STR("|-#-| "), 4, &arena);
  cebus_assert(str_eq(tf_fleet, STR("|-#-| |-#-| |-#-| |-#-| ")), "");
  arena_free(&arena);
}

static void test_reverse(void) {
  Arena arena = {0};
  Str s = str_reverse(STR("Hello, World"), &arena);
  cebus_assert(str_eq(s, STR("dlroW ,olleH")), "");
  arena_free(&arena);
}

static void test_hash(void) {
  const struct {
    Str s;
    u64 hash;
  } tests[] = {
      {STR("Hello"), 0x63f0bfacf2c00f6b},
      {STR("This is a very long string"), 0xf276220f1fa71f6b},
      {STR("Another"), 0x90c0e53673f3e9f4},
      {STR("Hello"), 0x63f0bfacf2c00f6b},
  };

  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    const u64 hash = str_hash(tests[i].s);
    cebus_assert(hash == tests[i].hash, "'" STR_FMT "': has 0x%" U64_HEX " expected 0x%" U64_HEX,
                 STR_ARG(tests[i].s), hash, tests[i].hash);
  }
}

static void test_format(void) {
  Arena arena = {0};
  Str s = str_format(&arena, "%d %d", 420, 69);
  cebus_assert(str_eq(s, STR("420 69")), "Should format correctly!");
  arena_free(&arena);
}

static void test_take(void) {
  Str s = STR("what a wonderful string");
  cebus_assert(str_eq(str_take(&s, 2), STR("wh")), STR_REPR, STR_ARG(s));
  cebus_assert(str_eq(str_take_right(&s, 2), STR("ng")), STR_REPR, STR_ARG(s));

  cebus_assert(str_eq(str_take(&s, 8), STR("at a won")), STR_REPR, STR_ARG(s));
  cebus_assert(str_eq(str_take_right(&s, 5), STR(" stri")), STR_REPR, STR_ARG(s));

  cebus_assert(str_eq(s, STR("derful")), STR_REPR, STR_ARG(s));
}

static void test_try_take(void) {
  Str s = STR("what a wonderful string");

  Str chunk = {0};
  cebus_assert(str_try_take(&s, 7, &chunk) == true, STR_REPR, STR_ARG(s));
  cebus_assert(str_eq(chunk, STR("what a ")), STR_REPR, STR_ARG(s));

  cebus_assert(str_try_take_right(&s, 7, &chunk) == true, STR_REPR, STR_ARG(s));
  cebus_assert(str_eq(chunk, STR(" string")), STR_REPR, STR_ARG(s));

  cebus_assert(str_eq(s, STR("wonderful")), STR_REPR, STR_ARG(s));
  cebus_assert(str_try_take_right(&s, 9, &chunk) == true, STR_REPR, STR_ARG(s));

  cebus_assert(str_try_take_right(&s, 2, &chunk) == false, STR_REPR, STR_ARG(s));
}

int main(void) {
  test_compare();
  test_transform();
  test_copy();
  test_append();
  test_trim();
  test_chop();
  test_try_chop();
  test_chop_right();
  test_number_converting();
  test_find();
  test_count();
  test_replace();
  test_substring();
  test_join();
  test_justify();
  test_cmp();
  test_repeat();
  test_reverse();
  test_hash();
  test_format();
  test_take();
  test_try_take();
}
