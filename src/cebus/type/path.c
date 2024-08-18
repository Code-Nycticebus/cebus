#include "path.h"
#include "cebus/type/string.h"

Path path_join(Arena *arena, usize size, Path *paths) {
  return str_join(STR("/"), size, paths, arena);
}

Path path_join_da(Arena *arena, PathDa *da) { return path_join(arena, da->len, da->items); }

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
