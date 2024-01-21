#include "core/asserts.h"
#include "os/fs.h"

#include "collections/vec.h"
#include "core/defines.h"
#include "types/str.h"

int main(void) {
  Arena arena = {0};

  clib_assert(file_exists(STR(__FILE__)), "This file should exist");

  file_write(STR("test"), BYTES(0x69, 0x69), ErrThrow);
  clib_assert(file_exists(STR("test")), "This file should exist");

  file_rename(STR("test"), STR("test.2"), ErrThrow);
  clib_assert(file_exists(STR("test.2")), "This file should exist");

  file_remove(STR("test.2"), ErrThrow);
  clib_assert(file_exists(STR("test.2")) == false,
              "This file should not exist");

  Str content = file_read_str(STR(__FILE__), &arena, ErrThrow);

  VEC(Str) list = {0};
  vec_init(&list, &arena);
  for (Str line = {0}; str_try_chop_by_delim(&content, '\n', &line);) {
    vec_push(&list, line);
  }

  Str new = str_join_suffix(STR("\n"), list.len, list.items, &arena);
  file_write(STR(__FILE__), str_to_bytes(new), ErrThrow);

  arena_free(&arena);
}
