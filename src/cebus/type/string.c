#include "./string.h"

#include "cebus/core/arena.h"
#include "cebus/type/byte.h"
#include "cebus/type/char.h"
#include "cebus/type/integer.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////

Str str_from_parts(usize size, const char *cstr) { return (Str){.len = size, .data = cstr}; }

Str str_from_bytes(Bytes bytes) { return str_from_parts(bytes.size, (const char *)bytes.data); }

Bytes str_to_bytes(Str s) { return bytes_from_parts(s.len, s.data); }

Str str_from_cstr(const char *cstr) { return (Str){.len = strlen(cstr), .data = cstr}; }

Str str_format(Arena *arena, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  usize size = (usize)vsnprintf(NULL, 0, fmt, va) + 1;
  va_end(va);

  char *buffer = arena_calloc(arena, size);
  va_start(va, fmt);
  vsnprintf(buffer, size, fmt, va);
  va_end(va);
  return str_from_parts(size - 1, buffer);
}

///////////////////////////////////////////////////////////////////////////////

Str str_copy(Str s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.len + 1);
  memcpy(buffer, s.data, s.len);
  buffer[s.len] = '\0';
  return str_from_parts(s.len, buffer);
}

Str str_append(Str s, Str suffix, Arena *arena) {
  const usize new_size = s.len + suffix.len;
  char *buffer = arena_alloc(arena, new_size + 1);
  memcpy(&buffer[0], s.data, s.len);
  memcpy(&buffer[s.len], suffix.data, suffix.len);
  buffer[new_size] = '\0';
  return str_from_parts(new_size, buffer);
}

Str str_prepend(Str s, Str prefix, Arena *arena) {
  const usize new_size = s.len + prefix.len;
  char *buffer = arena_alloc(arena, new_size + 1);
  memcpy(&buffer[0], prefix.data, prefix.len);
  memcpy(&buffer[prefix.len], s.data, s.len);
  buffer[new_size] = '\0';
  return str_from_parts(new_size, buffer);
}

Str str_wrap(Str s, Str wrap, Arena *arena) {
  const usize new_size = s.len + wrap.len * 2;
  char *buffer = arena_alloc(arena, new_size + 1);
  memcpy(&buffer[0], wrap.data, wrap.len);
  memcpy(&buffer[wrap.len], s.data, s.len);
  memcpy(&buffer[wrap.len + s.len], wrap.data, wrap.len);
  buffer[new_size] = '\0';
  return str_from_parts(new_size, buffer);
}

Str str_join(Str sep, usize count, Str *s, Arena *arena) {
  if (count == 0) {
    return (Str){0};
  }
  usize size = sep.len * (count - 1);
  for (usize i = 0; i < count; i++) {
    size += s[i].len;
  }
  char *buffer = arena_alloc(arena, size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    if (i != 0) {
      memcpy(&buffer[b_idx], sep.data, sep.len);
      b_idx += sep.len;
    }
    memcpy(&buffer[b_idx], s[i].data, s[i].len);
    b_idx += s[i].len;
  }
  buffer[size] = '\0';

  return str_from_parts(size, buffer);
}

Str str_join_suffix(Str suffix, usize count, Str *s, Arena *arena) {
  usize size = suffix.len * count;
  for (usize i = 0; i < count; i++) {
    size += s[i].len;
  }
  char *buffer = arena_alloc(arena, size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    memcpy(&buffer[b_idx], s[i].data, s[i].len);
    b_idx += s[i].len;
    memcpy(&buffer[b_idx], suffix.data, suffix.len);
    b_idx += suffix.len;
  }
  buffer[size] = '\0';

  return str_from_parts(size, buffer);
}

Str str_join_prefix(Str prefix, usize count, Str *s, Arena *arena) {
  usize size = prefix.len * count;
  for (usize i = 0; i < count; i++) {
    size += s[i].len;
  }
  char *buffer = arena_alloc(arena, size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    memcpy(&buffer[b_idx], prefix.data, prefix.len);
    b_idx += prefix.len;
    memcpy(&buffer[b_idx], s[i].data, s[i].len);
    b_idx += s[i].len;
  }
  buffer[size] = '\0';

  return str_from_parts(size, buffer);
}

Str str_join_wrap(Str sep, Str wrap, usize count, Str *s, Arena *arena) {
  usize size = sep.len * (count - 1) + wrap.len * count * 2;
  for (usize i = 0; i < count; i++) {
    size += s[i].len;
  }
  char *buffer = arena_alloc(arena, size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    if (i != 0) {
      memcpy(&buffer[b_idx], sep.data, sep.len);
      b_idx += sep.len;
    }
    memcpy(&buffer[b_idx], wrap.data, wrap.len);
    b_idx += wrap.len;
    memcpy(&buffer[b_idx], s[i].data, s[i].len);
    b_idx += s[i].len;
    memcpy(&buffer[b_idx], wrap.data, wrap.len);
    b_idx += wrap.len;
  }
  buffer[size] = '\0';

  return str_from_parts(size, buffer);
}

Str str_upper(Str s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.len + 1);
  buffer[s.len] = '\0';
  for (usize i = 0; i < s.len; i++) {
    buffer[i] = c_to_upper(s.data[i]);
  }
  return str_from_parts(s.len, buffer);
}

Str str_lower(Str s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.len + 1);
  buffer[s.len] = '\0';
  for (usize i = 0; i < s.len; i++) {
    buffer[i] = c_to_lower(s.data[i]);
  }
  return str_from_parts(s.len, buffer);
}

Str str_replace(Str s, Str old, Str new, Arena *arena) {
  usize count = str_count(s, old);
  usize new_size = (s.len - (old.len * count) + (count * new.len));
  char *buffer = arena_alloc(arena, new_size + 1);
  buffer[new_size] = '\0';

  for (usize i = 0, j = 0; i < s.len;) {
    if (old.len <= s.len - i && memcmp(&s.data[i], old.data, old.len) == 0) {
      memcpy(&buffer[j], new.data, new.len);
      i += old.len;
      j += new.len;
    } else {
      buffer[j++] = s.data[i++];
    }
  }

  return str_from_parts(new_size, buffer);
}

Str str_center(Str s, usize width, char fillchar, Arena *arena) {
  if (width < s.len) {
    return str_copy(s, arena);
  }
  char *buffer = arena_calloc(arena, width + 1);
  const usize left_width = (width - s.len) / 2;
  const usize right_width = (width - s.len - left_width);
  usize idx = 0;
  for (usize i = 0; i < left_width; i++) {
    buffer[idx++] = fillchar;
  }
  for (usize i = 0; i < s.len; i++) {
    buffer[idx++] = s.data[i];
  }
  for (usize i = 0; i < right_width; i++) {
    buffer[idx++] = fillchar;
  }
  return str_from_parts(width, buffer);
}

Str str_ljust(Str s, usize width, char fillchar, Arena *arena) {
  if (width < s.len) {
    return str_copy(s, arena);
  }
  char *buffer = arena_calloc(arena, width + 1);
  usize idx = 0;
  for (usize i = 0; i < s.len; i++) {
    buffer[idx++] = s.data[i];
  }
  for (usize i = 0; i < width - s.len; i++) {
    buffer[idx++] = fillchar;
  }
  return str_from_parts(width, buffer);
}

Str str_rjust(Str s, usize width, char fillchar, Arena *arena) {
  if (width < s.len) {
    return str_copy(s, arena);
  }
  char *buffer = arena_calloc(arena, width + 1);
  usize idx = 0;
  for (usize i = 0; i < width - s.len; i++) {
    buffer[idx++] = fillchar;
  }
  for (usize i = 0; i < s.len; i++) {
    buffer[idx++] = s.data[i];
  }

  return str_from_parts(width, buffer);
}

Str str_repeat(Str s, usize count, Arena *arena) {
  usize len = s.len * count;
  char *buffer = arena_alloc(arena, len + 1);
  buffer[len] = '\0';

  usize idx = 0;
  for (usize i = 0; i < count; i++) {
    for (usize j = 0; j < s.len; j++) {
      buffer[idx++] = s.data[j];
    }
  }

  return str_from_parts(len, buffer);
}

Str str_reverse(Str s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.len + 1);
  buffer[s.len] = '\0';
  for (usize i = 0; i < s.len; i++) {
    buffer[i] = s.data[s.len - i - 1];
  }
  return str_from_parts(s.len, buffer);
}

///////////////////////////////////////////////////////////////////////////////

bool str_eq(Str s1, Str s2) {
  if (s1.len != s2.len) {
    return false;
  }
  return memcmp(s1.data, s2.data, s1.len) == 0;
}

bool str_eq_ignorecase(Str s1, Str s2) {
  if (s1.len != s2.len) {
    return false;
  }
  for (size_t i = 0; i < s1.len; i++) {
    if (c_to_lower(s1.data[i]) != c_to_lower(s2.data[i])) {
      return false;
    }
  }
  return true;
}

bool str_startswith(Str s1, Str prefix) {
  if (s1.len < prefix.len) {
    return false;
  }
  return memcmp(s1.data, prefix.data, prefix.len) == 0;
}

bool str_endswith(Str s1, Str suffix) {
  if (s1.len < suffix.len) {
    return false;
  }
  const usize idx = s1.len - suffix.len;
  return memcmp(&s1.data[idx], suffix.data, suffix.len) == 0;
}

bool str_contains(Str haystack, Str needle) {
  if (haystack.len < needle.len) {
    return false;
  }
  for (usize i = 0; i < haystack.len - needle.len + 1; i++) {
    if (memcmp(&haystack.data[i], needle.data, needle.len) == 0) {
      return true;
    }
  }
  return false;
}

bool str_includes(Str haystack, char needle) {
  if (haystack.len == 0) {
    return false;
  }
  for (usize i = 0; i < haystack.len; i++) {
    if (haystack.data[i] == needle) {
      return true;
    }
  }
  return false;
}

bool str_empty(Str s) { return s.len == 0; }

CmpOrdering str_compare_gt(Str s1, Str s2) {
  const usize min_bytes = usize_min(s1.len, s2.len);
  const int r = memcmp(s1.data, s2.data, min_bytes);
  return r < 0   ? CMP_LESS    // less
         : 0 < r ? CMP_GREATER // greater
                 : CMP_EQUAL;  // equal
}

CmpOrdering str_compare_lt(Str s1, Str s2) { return str_compare_gt(s2, s1); }

static CmpOrdering _str_cmp_gt(const void *s1, const void *s2) {
  return str_compare_gt(*(const Str *)s1, *(const Str *)s2);
}

static CmpOrdering _str_cmp_lt(const void *s1, const void *s2) {
  return str_compare_gt(*(const Str *)s2, *(const Str *)s1);
}

///////////////////////////////////////////////////////////////////////////////

CompareFn str_compare_qsort(CmpOrdering ordering) {
  return ordering == CMP_LESS ? _str_cmp_lt : _str_cmp_gt;
}

Str str_trim_left(Str s) {
  Str result = s;
  for (usize i = 0; i < s.len && c_is_space(s.data[i]); ++i) {
    result.data++;
    result.len--;
  }
  return result;
}

Str str_trim_left_by_delim(Str s, char delim) {
  Str result = s;
  for (usize i = 0; i < s.len && s.data[i] == delim; ++i) {
    result.data++;
    result.len--;
  }
  return result;
}

Str str_trim_left_by_predicate(Str s, bool (*predicate)(char)) {
  Str result = s;
  for (usize i = 0; i < s.len && predicate(s.data[i]); ++i) {
    result.data++;
    result.len--;
  }
  return result;
}

Str str_trim_right(Str s) {
  Str result = s;
  for (usize i = 0; i < s.len && c_is_space(s.data[s.len - i - 1]); ++i) {
    result.len--;
  }
  return result;
}

Str str_trim_right_by_delim(Str s, char delim) {
  Str result = s;
  for (usize i = 0; i < s.len && s.data[s.len - i - 1] == delim; ++i) {
    result.len--;
  }
  return result;
}

Str str_trim_right_by_predicate(Str s, bool (*predicate)(char)) {
  Str result = s;
  for (usize i = 0; i < s.len && predicate(s.data[s.len - i - 1]); ++i) {
    result.len--;
  }
  return result;
}

Str str_trim(Str s) { return str_trim_left(str_trim_right(s)); }
Str str_trim_by_delim(Str s, char delim) {
  return str_trim_left_by_delim(str_trim_right_by_delim(s, delim), delim);
}
Str str_trim_by_predicate(Str s, bool (*predicate)(char)) {
  return str_trim_left_by_predicate(str_trim_right_by_predicate(s, predicate), predicate);
}

bool str_try_chop_by_delim(Str *s, char delim, Str *chunk) {
  if (s->len == 0) {
    return false;
  }
  usize i = 0;
  while (i < s->len && s->data[i] != delim) {
    ++i;
  }

  if (s->len && i <= s->len) {
    if (chunk) {
      *chunk = str_from_parts(i, s->data);
    }
    const usize new_len = usize_min(s->len, i + 1);
    s->data += new_len;
    s->len -= new_len;
    *s = str_trim_left_by_delim(*s, delim);
    return true;
  }

  return false;
}

Str str_chop_by_delim(Str *s, char delim) {
  usize i = 0;
  while (i < s->len && s->data[i] != delim) {
    ++i;
  }

  if (s->len && i <= s->len) {
    Str chunk = str_from_parts(i, s->data);
    const usize new_len = usize_min(s->len, i + 1);
    s->data += new_len;
    s->len -= new_len;
    *s = str_trim_left_by_delim(*s, delim);
    return chunk;
  }

  return *s;
}

bool str_try_chop_by_predicate(Str *s, bool (*predicate)(char), Str *chunk) {

  usize i = 0;
  while (i < s->len && !predicate(s->data[i])) {
    ++i;
  }

  if (s->len && i <= s->len) {
    if (chunk) {
      *chunk = str_from_parts(i, s->data);
    }
    const usize new_len = usize_min(s->len, i + 1);
    s->data += new_len;
    s->len -= new_len;
    *s = str_trim_left_by_predicate(*s, predicate);
    return true;
  }
  return false;
}

Str str_chop_by_predicate(Str *s, bool (*predicate)(char)) {
  usize i = 0;
  while (i < s->len && !predicate(s->data[i])) {
    ++i;
  }

  if (s->len && i <= s->len) {
    Str chunk = str_from_parts(i, s->data);
    const usize new_len = usize_min(s->len, i + 1);
    s->data += new_len;
    s->len -= new_len;
    *s = str_trim_left_by_predicate(*s, predicate);
    return chunk;
  }
  return *s;
}

Str str_chop_right_by_delim(Str *s, char delim) {
  usize i = 0;
  while (i < s->len && s->data[s->len - i - 1] != delim) {
    ++i;
  }

  if (s->len && i <= s->len) {
    Str chunk = str_from_parts(i, &s->data[s->len - i]);
    s->len -= usize_min(s->len, i + 1);
    *s = str_trim_right_by_delim(*s, delim);
    return chunk;
  }
  return *s;
}

Str str_chop_right_by_predicate(Str *s, bool (*predicate)(char)) {
  usize i = 0;
  while (i < s->len && !predicate(s->data[s->len - i - 1])) {
    ++i;
  }

  if (s->len && i <= s->len) {
    Str chunk = str_from_parts(i, &s->data[s->len - i]);
    s->len -= usize_min(s->len, i + 1);
    *s = str_trim_right_by_predicate(*s, predicate);
    return chunk;
  }
  return *s;
}

Str str_substring(Str s, usize start, usize end) {
  if (end <= start || s.len <= start || s.len < end) {
    return STR("");
  }
  return str_from_parts(end - start, &s.data[start]);
}

Str str_take(Str *s, usize count) {
  count = usize_min(s->len, count);
  Str ret = str_from_parts(count, s->data);
  s->len -= count;
  s->data += count;
  return ret;
}

bool str_try_take(Str *s, usize count, Str *chunk) {
  if (s->len == 0) {
    return false;
  }
  count = usize_min(s->len, count);
  *chunk = str_from_parts(count, s->data);
  s->len -= count;
  s->data += count;
  return true;
}

Str str_take_right(Str *s, usize count) {
  count = usize_min(s->len, count);
  Str ret = str_from_parts(count, &s->data[s->len - count]);
  s->len -= count;
  return ret;
}

bool str_try_take_right(Str *s, usize count, Str *chunk) {
  if (s->len == 0) {
    return false;
  }
  count = usize_min(s->len, count);
  *chunk = str_from_parts(count, &s->data[s->len - count]);
  s->len -= count;
  return true;
}

///////////////////////////////////////////////////////////////////////////////

u64 str_u64(Str s) {
  const int radix = 10;
  Arena arena = {0};
  Str owned = str_copy(s, &arena);
  u64 value = strtoull(owned.data, NULL, radix);
  arena_free(&arena);
  return value;
}

u64 str_chop_u64(Str *s) {
  const int radix = 10;
  Arena arena = {0};
  Str owned = str_copy(*s, &arena);
  char *endptr;
  u64 value = strtoull(owned.data, &endptr, radix);

  const usize size = (usize)(endptr - owned.data);
  s->data += size;
  s->len -= size;

  arena_free(&arena);
  return value;
}

i64 str_i64(Str s) {
  const int radix = 10;
  Arena arena = {0};
  Str owned = str_copy(s, &arena);
  i64 value = strtoll(owned.data, NULL, radix);
  arena_free(&arena);
  return value;
}

i64 str_chop_i64(Str *s) {
  const int radix = 10;
  Arena arena = {0};
  Str owned = str_copy(*s, &arena);
  char *endptr;
  i64 value = strtoll(owned.data, &endptr, radix);

  const usize size = (usize)(endptr - owned.data);
  s->data += size;
  s->len -= size;

  arena_free(&arena);
  return value;
}

f64 str_f64(Str s) {
  Arena arena = {0};
  Str owned = str_copy(s, &arena);
  double value = strtod(owned.data, NULL);
  arena_free(&arena);
  return value;
}

f64 str_chop_f64(Str *s) {
  Arena arena = {0};
  Str owned = str_copy(*s, &arena);

  char *endptr;
  f64 value = strtod(owned.data, &endptr);

  const usize size = (usize)(endptr - owned.data);
  s->data += size;
  s->len -= size;

  arena_free(&arena);
  return value;
}

usize str_find(Str haystack, Str needle) {
  if (haystack.len < needle.len) {
    return STR_NOT_FOUND;
  }
  for (usize i = 0; i < haystack.len - needle.len + 1; i++) {
    if (memcmp(&haystack.data[i], needle.data, needle.len) == 0) {
      return i;
    }
  }
  return STR_NOT_FOUND;
}

usize str_find_last(Str haystack, Str needle) {
  if (haystack.len < needle.len) {
    return STR_NOT_FOUND;
  }
  for (usize i = haystack.len - needle.len + 1; i > 0; i--) {
    if (memcmp(&haystack.data[i - 1], needle.data, needle.len) == 0) {
      return i - 1;
    }
  }
  return STR_NOT_FOUND;
}

usize str_count(Str haystack, Str needle) {
  usize count = 0;
  if (haystack.len < needle.len) {
    return count;
  }
  for (usize i = 0; i < haystack.len - needle.len + 1; i++) {
    if (memcmp(&haystack.data[i], needle.data, needle.len) == 0) {
      count++;
      i += needle.len - 1;
    }
  }
  return count;
}

char str_getc(Str s, usize idx) {
  if (s.len <= idx) {
    return '\0';
  }
  return s.data[idx];
}

u64 str_hash(Str s) {
  const uint64_t magic_prime = 0x00000100000001b3;
  uint64_t hash = 0xcbf29ce484222325; // NOLINT
  for (usize i = 0; i < s.len; ++i) {
    hash = (hash ^ (u64)s.data[i]) * magic_prime;
  }
  return hash;
}

///////////////////////////////////////////////////////////////////////////////
