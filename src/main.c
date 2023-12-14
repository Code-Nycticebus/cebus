#include "arena.h"
#include "da.h"
#include "str.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

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

  arena_free(arena);
}
