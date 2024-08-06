#include "path.h"
#include "cebus/type/string.h"

#include <stdarg.h>

Path _path_new(Arena *arena, ...) {
  Arena scratch = {0};
  DA(Path) paths = da_new(&scratch);
  va_list va;
  va_start(va, arena);
  Path path = va_arg(va, Path);
  while (path.data != NULL) {
    if (path.len) {
      da_push(&paths, path);
    }
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
  if (str_eq(path, STR("."))) {
    return STR("");
  }
  return str_chop_right_by_delim(&path, '/');
}

Str path_suffix(Path path) {
  if (str_eq(path, STR("."))) {
    return STR("");
  }
  Str name = str_chop_right_by_delim(&path, '/');
  usize idx = str_find_last(name, STR("."));
  if (idx == STR_NOT_FOUND) {
    return STR("");
  }
  return str_substring(name, idx, name.len);
}

Str path_stem(Path path) {
  Str name = str_chop_right_by_delim(&path, '/');
  usize idx = str_find_last(name, STR("."));
  if (idx == STR_NOT_FOUND) {
    return name;
  }
  return str_substring(name, 0, idx);
}

Path path_parent(Path path) {
  (void)str_chop_right_by_delim(&path, '/');
  return path;
}
