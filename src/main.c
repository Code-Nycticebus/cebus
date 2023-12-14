#include "arena.h"
#include "da.h"
#include "str.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

bool illegal(char c) { return c == '\n'; }

typedef struct {
  DA(size_t);
} ListSize;

int main(void) {
  ListSize list = {0};
  da_init(&list, 2);
  for (size_t i = 0; i < 10; ++i) {
    da_push(&list, i + 1);
  }

  for (size_t i = 0; i < list.len; ++i) {
    assert(list.items[i] == i + 1);
  }

  da_free(&list);

  Arena *arena = arena_make();

  Str str = STR("Hello");
  Str str2 = str_copy(arena, str);
  Str str3 = str_from_cstr(", World");
  Str full = str_cat(arena, str2, str3);

  assert(str_eq(full, STR("Hello, World")));
  assert(str_startswith(full, STR("Hello, ")));
  assert(str_endswith(full, STR(" World")));

  Str dirty = STR("\t  Hello World  \n");
  Str trim_l = str_trim_left(dirty);
  assert(str_eq(trim_l, STR("Hello World  \n")));

  Str trim_r = str_trim_right(dirty);
  assert(str_eq(trim_r, STR("\t  Hello World")));

  Str trim = str_trim(dirty);
  assert(str_eq(trim, STR("Hello World")));

  Str text = STR("Hello\nThis is text that i want splitted\nI dont want the "
                 "newlines so i can do some nice shit.\n");

  for (Str s = text, line = {0};
       str_try_chop_by_predicate(&s, illegal, &line);) {
    printf("\t" STR_FMT "\n", STR_ARG(line));
  }

  arena_free(arena);
}
