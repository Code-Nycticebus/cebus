#include "str.h"
#include "arena.h"
#include "inttypes.h"

#include <ctype.h>
#include <string.h>

Str str_from_parts(size_t size, const char *cstr) {
  return (Str){.len = size, .data = cstr};
}

Str str_from_cstr(const char *cstr) {
  return (Str){.len = strlen(cstr), .data = cstr};
}

Str str_trim_left(Str str) {
  Str result = str;
  for (size_t i = 0; i < str.len && isspace(str.data[i]); ++i) {
    result.data++;
    result.len--;
  }
  return result;
}

Str str_trim_right(Str str) {
  Str result = str;
  for (size_t i = 0; i < str.len && isspace(str.data[str.len - i - 1]); ++i) {
    result.len--;
  }
  return result;
}

Str str_trim(Str str) { return str_trim_left(str_trim_right(str)); }

Str str_copy(Arena *arena, Str src) {
  char *buffer = arena_alloc(arena, src.len + 1);
  strncpy(buffer, src.data, src.len);
  buffer[src.len] = '\0';
  return str_from_parts(src.len, buffer);
}

Str str_cat(Arena *arena, Str s1, Str s2) {
  const size_t new_size = s1.len + s2.len;
  char *buffer = arena_calloc(arena, new_size + 1);
  strncpy(buffer, s1.data, s1.len);
  strncat(buffer, s2.data, s2.len);
  buffer[new_size] = '\0';
  return str_from_parts(new_size, buffer);
}

bool str_eq(Str s1, Str s2) {
  if (s1.len != s2.len) {
    return false;
  }
  return strncmp(s1.data, s2.data, s1.len) == 0;
}

bool str_startswith(Str s1, Str prefix) {
  if (s1.len < prefix.len) {
    return false;
  }
  return strncmp(s1.data, prefix.data, prefix.len) == 0;
}

bool str_endswith(Str s1, Str suffix) {
  if (s1.len < suffix.len) {
    return false;
  }
  size_t idx = s1.len - suffix.len;
  return strncmp(&s1.data[idx], suffix.data, suffix.len) == 0;
}

bool str_try_chop_by_delim(Str *str, char delim, Str *chunk) {
  size_t i = 0;
  while (i < str->len && str->data[i] != delim) {
    ++i;
  }

  if (i < str->len) {
    *chunk = str_from_parts(i, str->data);
    str->data += i + 1;
    str->len -= i + 1;
    return true;
  }
  return false;
}

Str str_chop_by_delim(Str *str, char delim) {
  size_t i = 0;
  while (i < str->len && str->data[i] != delim) {
    ++i;
  }

  if (i < str->len) {
    Str chunk = str_from_parts(i, str->data);
    str->data += i + 1;
    str->len -= i + 1;
    return chunk;
  }
  return *str;
}

bool str_try_chop_by_predicate(Str *str, bool (*predicate)(char), Str *chunk) {
  size_t i = 0;
  while (i < str->len && !predicate(str->data[i])) {
    ++i;
  }

  if (i < str->len) {
    *chunk = str_from_parts(i, str->data);
    str->data += i + 1;
    str->len -= i + 1;
    return true;
  }
  return false;
}

Str str_chop_by_predicate(Str *str, bool (*predicate)(char)) {
  size_t i = 0;
  while (i < str->len && !predicate(str->data[i])) {
    ++i;
  }

  if (i < str->len) {
    Str chunk = str_from_parts(i, str->data);
    str->data += i + 1;
    str->len -= i + 1;
    return chunk;
  }
  return *str;
}

uint64_t str_to_u64(Str str) {
  uint64_t result = 0;
  for (size_t i = 0; i < str.len && isdigit(str.data[i]); ++i) {
    result = result * 10 + (uint64_t)str.data[i] - '0';
  }
  return result;
}

uint64_t str_chop_u64(Str *str) {
  uint64_t result = 0;
  size_t i = 0;
  for (; i < str->len && isdigit(str->data[i]); ++i) {
    result = result * 10 + (uint64_t)str->data[i] - '0';
  }
  str->len -= i;
  str->data += i;
  return result;
}
