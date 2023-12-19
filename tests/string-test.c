#include "clib/str.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

bool sep(char c) { return isspace(c) || isblank(c); }

void test_compare(void) {
  Str s = STR("Hello, World");

  assert(str_eq(s, STR("Hello, World")) == true);
  assert(str_eq_ignorecase(s, STR("hello, world")) == true);
  assert(str_startswith(s, STR("Hello, ")) == true);
  assert(str_endswith(s, STR(", World")) == true);

  assert(str_contains(s, STR("Hell")) == true);
  assert(str_contains(s, STR("TEST")) == false);
}

static char mock(size_t idx, char c) {
  const size_t upper_cases[] = {3, 7, 9, 11};
  for (size_t i = 0; i < 4; i++) {
    if (idx == upper_cases[i]) {
      return toupper(c);
    }
  }
  return tolower(c);
}

void test_transform(void) {
  Arena arena = {0};
  Str s = STR("Hello, World");
  Str lower = str_lower(s, &arena);
  Str upper = str_upper(s, &arena);

  Str mapped = str_map(s, mock, &arena);

  assert(str_eq(lower, STR("hello, world")));
  assert(str_eq(upper, STR("HELLO, WORLD")));
  assert(str_eq(mapped, STR("helLo, WoRlD")));

  arena_free(&arena);
}

void test_copy(void) {
  Arena arena = {0};

  Str str = STR("Hello");
  Str str2 = str_copy(str, &arena);
  Str str3 = str_from_cstr(", World");
  Str full = str_concat(str2, str3, &arena);

  assert(str_eq(full, STR("Hello, World")));
  assert(str_getc(full, 0) == 'H');
  assert(str_getc(full, 1) == 'e');
  assert(str_getc(full, 7) == 'W');
  assert(str_getc(full, full.len) == '\0');

  arena_free(&arena);
}

void test_trim(void) {
  Str dirty = STR("\t  Hello World  \n");
  Str trim_l = str_trim_left(dirty);
  assert(str_eq(trim_l, STR("Hello World  \n")));

  Str trim_r = str_trim_right(dirty);
  assert(str_eq(trim_r, STR("\t  Hello World")));

  Str trim = str_trim(dirty);
  assert(str_eq(trim, STR("Hello World")));
}

void test_chop(void) {
  Str text = STR("Hello\nThis is text");
  Str h = str_chop_by_delim(&text, '\n');
  Str rest = str_chop_by_predicate(&text, sep);
  assert(str_eq(h, STR("Hello")));
  assert(str_eq(rest, STR("This")));
  assert(str_eq(text, STR("is text")));
}

void test_try_chop(void) {
  Str text = STR("Hello\nThis is text");
  Str h = {0};
  bool t1 = str_try_chop_by_delim(&text, '\n', &h);
  assert(t1 == true);
  Str rest = {0};
  bool t2 = str_try_chop_by_predicate(&text, sep, &rest);
  assert(t2 == true);

  assert(str_eq(h, STR("Hello")));
  assert(str_eq(rest, STR("This")));
  assert(str_eq(text, STR("is text")));

  bool t3 = str_try_chop_by_delim(&text, '\n', &rest);
  assert(t3 == false);
}

void test_chop_right(void) {
  Str text = STR("Hello\nThis is text");
  Str t = str_chop_right_by_predicate(&text, sep);
  Str rest = str_chop_right_by_delim(&text, '\n');
  Str rest2 = str_chop_right_by_delim(&text, '\n');

  assert(str_eq(t, STR("text")));
  assert(str_eq(rest, STR("This is")));
  assert(str_eq(rest2, STR("Hello")));
}

void test_u64(void) {
  Arena arena = {0};
  const size_t N = 64;
  Str number = str_u64(&arena, N);
  assert(str_eq(number, STR("64")));

  Str n = str_concat(number, STR(" bytes"), &arena);
  assert(str_eq(n, STR("64 bytes")));

  assert(str_to_u64(n) == 64);
  assert(str_chop_u64(&n) == 64);
  assert(str_eq(n, STR(" bytes")));

  arena_free(&arena);
}

void test_find(void) {
  Str s = STR("Hello, World");
  assert(str_find(s, STR("Hello")) == 0);
  assert(str_find_last(s, STR("Hello")) == 0);
  assert(str_find(s, STR("World")) == 7);
  assert(str_find_last(s, STR("World")) == 7);
  assert(str_find(s, STR("TEST")) == STR_NOT_FOUND);
}

void test_count(void) {
  Str s = STR("Hello, World");
  size_t c = str_count(s, STR("o"));
  assert(c == 2);
  c = str_count(s, STR("TEST"));
  assert(c == 0);
}

void test_replace(void) {
  Arena arena = {0};

  Str s = STR("Hello, World");
  Str goodbye = str_replace(s, STR("Hello"), STR("Goodbye"), &arena);
  Str all = str_replace(s, STR("World"), STR("All!"), &arena);

  assert(str_eq(s, STR("Hello, World")));
  assert(str_eq(goodbye, STR("Goodbye, World")));
  assert(str_eq(all, STR("Hello, All!")));

  Str max_test = STR("test test test");
  Str result = str_replace(max_test, STR("test"), STR("result"), &arena);
  assert(str_eq(result, STR("result result result")));

  Str dash = STR("c-language");
  Str res = str_replace(dash, STR("-"), STR(""), &arena);
  assert(str_eq(res, STR("clanguage")));

  arena_free(&arena);
}

void test_substring(void) {
  Str s = STR("Hello, World");
  Str substring = str_substring(s, 0, 4);
  Str invalid1 = str_substring(s, 4, 2);
  Str invalid2 = str_substring(s, s.len, 2);

  assert(str_eq(substring, STR("Hell")));
  assert(str_eq(invalid1, STR("")));
  assert(str_eq(invalid2, STR("")));
}

void test_join(void) {
  Arena arena = {0};
  Str res =
      str_join(STR(", "), 2, (Str[2]){STR("Hello"), STR("World")}, &arena);
  assert(str_eq(res, STR("Hello, World")));
  arena_free(&arena);
}

void test_justify(void) {
  Arena arena = {0};
  const size_t width = 10;
  Str center = str_center(STR("Hello"), width, ' ', &arena);
  assert(str_eq(center, STR("  Hello   ")));
  Str left = str_ljust(STR("Hello"), width, ' ', &arena);
  assert(str_eq(left, STR("Hello     ")));
  Str right = str_rjust(STR("Hello"), width, ' ', &arena);
  assert(str_eq(right, STR("     Hello")));
  arena_free(&arena);
}

void test_cmp(void) {
  const size_t n = 4;
  Str array[] = {STR("Banana"), STR("Strawberry"), STR("Apple"), STR("Lemon")};
  qsort(array, n, sizeof(array[0]), str_compare_qsort);

  assert(str_eq(array[0], STR("Apple")));
  assert(str_eq(array[1], STR("Banana")));
  assert(str_eq(array[2], STR("Lemon")));
  assert(str_eq(array[3], STR("Strawberry")));
}

void test_repeat(void) {
  Arena arena = {0};
  Str tf_fleet = str_repeat(STR("|-#-| "), 4, &arena);
  assert(str_eq(tf_fleet, STR("|-#-| |-#-| |-#-| |-#-| ")));
  arena_free(&arena);
}

int main(void) {
  test_compare();
  test_transform();
  test_copy();
  test_trim();
  test_chop();
  test_chop_right();
  test_u64();
  test_find();
  test_count();
  test_replace();
  test_substring();
  test_join();
  test_justify();
  test_cmp();
  test_repeat();
}
