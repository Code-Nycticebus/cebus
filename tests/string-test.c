#include "str.h"

#include <assert.h>
#include <ctype.h>

bool sep(char c) { return isspace(c); }

void test_copy(void) {
  Arena *arena = arena_make();
  Str str = STR("Hello");
  Str str2 = str_copy(arena, str);
  Str str3 = str_from_cstr(", World");
  Str full = str_cat(arena, str2, str3);

  assert(str_eq(full, STR("Hello, World")));
  assert(str_startswith(full, STR("Hello, ")));
  assert(str_endswith(full, STR(" World")));

  arena_free(arena);
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
}

void test_u64(void) {
  Str n = STR("64 bytes");
  assert(str_to_u64(n) == 64);
  assert(str_chop_u64(&n) == 64);
  assert(str_eq(n, STR(" bytes")));
}

int main(void) {
  test_copy();
  test_trim();
  test_chop();
  test_u64();
}
