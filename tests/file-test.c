#include "collections/vec.h"
#include "core/defines.h"
#include "os/fs.h"
#include "types/str.h"

int main(void) {
  Arena arena = {0};
  Bytes content = file_read_bytes(STR(__FILE__), &arena, NULL);

  VEC(Str) list = {0};
  vec_init(&list, 10, &arena);
  for (Str line = {0}, s = str_from_bytes(content);
       str_try_chop_by_delim(&s, '\n', &line);) {
    vec_push(&list, line);
  }

  Str new = str_join_suffix(STR("\n"), list.len, list.items, &arena);
  file_write(STR(__FILE__), str_to_bytes(new), NULL);

  arena_free(&arena);
}
