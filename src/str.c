#include "str.h"
#include "arena.h"

#include <string.h>

Str str_from_parts(size_t size, const char *cstr) {
  return (Str){.len = size, .data = cstr};
}

Str str_from_cstr(const char *cstr) {
  return (Str){.len = strlen(cstr), .data = cstr};
}

Str str_copy(Arena *arena, Str src) {
  char *buffer = arena_alloc(arena, src.len + 1);
  strncpy(buffer, src.data, src.len);
  buffer[src.len] = '\0';
  return str_from_parts(src.len, buffer);
}

Str str_cat(Arena *arena, Str s1, Str s2) {
  const size_t new_size = s1.len + s2.len;
  char *buffer = arena_alloc(arena, new_size + 1);
  strncpy(buffer, s1.data, s1.len);
  strncat(buffer, s2.data, s2.len);
  return str_from_parts(new_size, buffer);
}
