#include "str.h"
#include "arena.h"

#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
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

Str str_copy(Str src, Arena *arena) {
  char *buffer = arena_alloc(arena, src.len + 1);
  strncpy(buffer, src.data, src.len);
  buffer[src.len] = '\0';
  return str_from_parts(src.len, buffer);
}

Str str_cat(Str s1, Str s2, Arena *arena) {
  const size_t new_size = s1.len + s2.len;
  char *buffer = arena_calloc(arena, new_size + 1);
  strncpy(buffer, s1.data, s1.len);
  strncat(buffer, s2.data, s2.len);
  buffer[new_size] = '\0';
  return str_from_parts(new_size, buffer);
}

Str str_upper(Str s, Arena *arena) {
  char *buffer = arena_calloc(arena, s.len + 1);
  for (size_t i = 0; i < s.len; i++) {
    buffer[i] = toupper(s.data[i]);
  }
  return str_from_parts(s.len, buffer);
}

Str str_lower(Str s, Arena *arena) {
  char *buffer = arena_calloc(arena, s.len + 1);
  for (size_t i = 0; i < s.len; i++) {
    buffer[i] = tolower(s.data[i]);
  }
  return str_from_parts(s.len, buffer);
}

Str str_map(Str s, char (*map_fn)(size_t, char), Arena *arena) {
  char *buffer = arena_calloc(arena, s.len + 1);
  for (size_t i = 0; i < s.len; i++) {
    buffer[i] = map_fn(i, s.data[i]);
  }
  return str_from_parts(s.len, buffer);
}

Str str_replace(Str s, Str old, Str new, Arena *arena) {
  size_t count = str_count(s, old);
  size_t new_size = (s.len - (old.len * count) + (new.len *count));
  char *buffer = arena_calloc(arena, new_size + 1);

  for (size_t i = 0, j = 0; i < s.len; i++, j++) {
    if (strncmp(&s.data[i], old.data, old.len) == 0) {
      strncpy(&buffer[j], new.data, new.len);
      i += old.len - 1;
      j += new.len - 1;
    } else {
      buffer[j] = s.data[i];
    }
  }

  return str_from_parts(new_size, buffer);
}

Str str_substring(Str s, size_t idx1, size_t idx2) {
  if (idx2 <= idx1 || s.len <= idx1) {
    return STR("");
  }
  return str_from_parts(idx2 - idx1, &s.data[idx1]);
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

bool str_contains(Str haystack, Str needle) {
  if (haystack.len < needle.len) {
    return false;
  }
  for (size_t i = 0; i < haystack.len - needle.len + 1; i++) {
    if (strncmp(&haystack.data[i], needle.data, needle.len) == 0) {
      return true;
    }
  }
  return false;
}

bool str_try_chop_by_delim(Str *str, char delim, Str *chunk) {
  size_t i = 0;
  while (i < str->len && str->data[i] != delim) {
    ++i;
  }

  if (i < str->len) {
    if (chunk) {
      *chunk = str_from_parts(i, str->data);
    }
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
    if (chunk) {
      *chunk = str_from_parts(i, str->data);
    }
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

Str str_chop_right_by_delim(Str *str, char delim) {
  size_t i = 0;
  while (i < str->len && str->data[str->len - i - 1] != delim) {
    ++i;
  }

  if (i < str->len) {
    Str chunk = str_from_parts(i, &str->data[str->len - i]);
    str->len -= i + 1;
    return chunk;
  }
  return *str;
}

Str str_chop_right_by_predicate(Str *str, bool (*predicate)(char)) {
  size_t i = 0;
  while (i < str->len && !predicate(str->data[str->len - i - 1])) {
    ++i;
  }

  if (i < str->len) {
    Str chunk = str_from_parts(i, &str->data[str->len - i]);
    str->len -= i + 1;
    return chunk;
  }
  return *str;
}

Str str_u64(Arena *arena, uint64_t n) {
  const size_t number_max_chars = 21;
  char *buffer = arena_alloc(arena, number_max_chars);
  size_t len = snprintf(buffer, number_max_chars, "%" PRIu64, n);
  return str_from_parts(len, buffer);
}

uint64_t str_to_u64(Str str) {
  uint64_t result = 0;
  for (size_t i = 0; i < str.len && isdigit(str.data[i]); ++i) {
    result = result * 10 + (uint64_t)str.data[i] - '0'; // NOLINT
  }
  return result;
}

uint64_t str_chop_u64(Str *str) {
  uint64_t result = 0;
  size_t i = 0;
  for (; i < str->len && isdigit(str->data[i]); ++i) {
    result = result * 10 + (uint64_t)str->data[i] - '0'; // NOLINT
  }
  str->len -= i;
  str->data += i;
  return result;
}

size_t str_find(Str haystack, Str needle) {
  if (haystack.len < needle.len) {
    return STR_NOT_FOUND;
  }
  for (size_t i = 0; i < haystack.len - needle.len + 1; i++) {
    if (strncmp(&haystack.data[i], needle.data, needle.len) == 0) {
      return i;
    }
  }
  return STR_NOT_FOUND;
}

size_t str_count(Str haystack, Str needle) {
  size_t count = 0;
  if (haystack.len < needle.len) {
    return count;
  }
  for (size_t i = 0; i < haystack.len - needle.len + 1; i++) {
    if (strncmp(&haystack.data[i], needle.data, needle.len) == 0) {
      count++;
      i += needle.len;
    }
  }
  return count;
}
