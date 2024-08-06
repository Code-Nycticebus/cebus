#include "path.h"
#include "cebus/collection/da.h"
#include "cebus/type/string.h"

#include <stdarg.h>

Path _path_new(Arena *arena, ...) {
  Arena scratch = {0};
  DA(Path) paths = da_new(&scratch);
  va_list va;
  va_start(va, arena);
  Path path = va_arg(va, Path);
  while (path.len > 0) {
    da_push(&paths, path);
    path = va_arg(va, Path);
  }
  va_end(va);

  Path fullpath = str_join(STR("/"), paths.len, paths.items, arena);
  arena_free(&scratch);
  return fullpath;
}

Path path_join(Arena *arena, PathDa *da) {
  // TODO: operating system dependent
  return str_join(STR("/"), da->len, da->items, arena);
}

Str path_name(Path path) {
  Str file = str_chop_right_by_delim(&path, '/');
  return str_chop_by_delim(&file, '.');
}

Path path_parent(Path path) {
  (void)str_chop_right_by_delim(&path, '/');
  return path;
}
