#include "utf8.h"

#include "cebus/core/arena.h"
#include "cebus/core/debug.h"
#include "cebus/type/byte.h"
#include "cebus/type/char.h"
#include "cebus/type/integer.h"

#include <string.h>

Utf8 utf8_decode(Bytes bytes, Error *error) {
  Utf8 str = {0};
  usize len = 0;
  for (usize i = 0; i < bytes.size; i++) {
    usize bit_count = u8_leading_ones(bytes.data[i]);
    if (!(bit_count <= 4)) {
      error_emit(error, UTF8_DECODE,
                 "Decoding utf-8 failed: too many starting bits at %" USIZE_FMT
                 ": found %" USIZE_FMT,
                 i, bit_count);
      return str;
    }
    usize idx = i;
    for (usize j = 1; j < bit_count; j++) {
      if (!(u8_leading_ones(bytes.data[idx + j]) == 1)) {
        error_emit(error, UTF8_DECODE,
                   "Decoding utf-8 failed: wrong bits in between at %" USIZE_FMT, idx + j);
        return str;
      }
      i++;
    }
    len++;
  }
  str.len = len;
  str.size = bytes.size;
  str.data = (const char *)bytes.data;
  return str;
}

Bytes utf8_encode(Utf8 s, Error *error) {
  bool ret = utf8_validate(s);
  if (ret == false) {
    error_emit(error, UTF8_ENCODE, "Encoding utf-8 failed");
    return (Bytes){0};
  }
  return bytes_from_parts(s.size, s.data);
}

bool utf8_eq(Utf8 s1, Utf8 s2) {
  if (s1.size != s2.size || s1.len != s2.len) {
    return false;
  }
  return memcmp(s1.data, s2.data, s1.size) == 0;
}

bool utf8_starts_with(Utf8 s, Utf8 prefix) {
  if (s.size < prefix.size) {
    return false;
  }
  return memcmp(s.data, prefix.data, prefix.size) == 0;
}

bool utf8_ends_with(Utf8 s, Utf8 prefix) {
  if (s.size < prefix.size) {
    return false;
  }
  return memcmp(&s.data[s.size - prefix.size], prefix.data, prefix.size) == 0;
}

bool utf8_try_next(Utf8 *str, Utf8 *out) {
  usize bit_count = u8_leading_ones((u8)str->data[0]);
  cebus_assert_return(bit_count <= 4, false);
  cebus_assert_return(bit_count != 1, false);
  usize bytes = usize_max(1, bit_count);
  cebus_assert_return(bytes <= str->size, false);
  *out = (Utf8){.len = 1, .size = bytes, .data = str->data};
  str->size -= bytes;
  str->len--;
  str->data += bytes;
  return true;
}

Utf8 utf8_next(Utf8 *str) {
  Utf8 res = {0};
  bool ret = utf8_try_next(str, &res);
  cebus_assert(ret, "Getting next character failed");
  return res;
}

Utf8 utf8_copy(Utf8 str, Arena *arena) {
  char *buffer = arena_alloc(arena, str.size + 1);
  buffer[str.size] = '\0';
  memcpy(buffer, str.data, str.size);
  return (Utf8){.len = str.len, .size = str.size, .data = buffer};
}

Utf8 utf8_append(Utf8 s, Utf8 suffix, Arena *arena) {
  const usize new_size = s.size + suffix.size;
  char *buffer = arena_alloc(arena, new_size + 1);
  buffer[new_size] = '\0';
  memcpy(&buffer[0], s.data, s.size);
  memcpy(&buffer[s.size], suffix.data, suffix.size);
  return (Utf8){.len = s.len + suffix.len, .size = new_size, .data = buffer};
}

Utf8 utf8_prepend(Utf8 s, Utf8 prefix, Arena *arena) {
  const usize new_size = s.size + prefix.size;
  char *buffer = arena_alloc(arena, new_size + 1);
  buffer[new_size] = '\0';
  memcpy(&buffer[0], prefix.data, prefix.size);
  memcpy(&buffer[prefix.size], s.data, s.size);
  return (Utf8){.len = s.len + prefix.len, .size = new_size, .data = buffer};
}

Utf8 utf8_join(Utf8 sep, usize count, Utf8 *s, Arena *arena) {
  usize new_size = (sep.size * (count - 1));
  usize new_len = (sep.len * (count - 1));
  for (usize i = 0; i < count; i++) {
    new_size += s[i].size;
    new_len += s[i].len;
  }
  char *buffer = arena_calloc(arena, new_size + 1);
  usize b_idx = 0;
  for (usize i = 0; i < count; i++) {
    if (i != 0) {
      memcpy(&buffer[b_idx], sep.data, sep.size);
      b_idx += sep.size;
    }
    memcpy(&buffer[b_idx], s[i].data, s[i].size);
    b_idx += s[i].size;
  }
  return (Utf8){.len = new_len, .size = new_size, .data = buffer};
}

Utf8 utf8_upper(Utf8 s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.size);
  for (usize i = 0; i < s.size; ++i) {
    usize bit_count = u8_leading_ones((u8)s.data[i]);
    if (bit_count == 0) {
      buffer[i] = c_to_upper(s.data[i]);
    } else {
      memcpy(&buffer[i], &s.data[i], bit_count);
      i += bit_count - 1;
    }
  }
  return (Utf8){.len = s.len, .size = s.size, .data = buffer};
}

Utf8 utf8_lower(Utf8 s, Arena *arena) {
  char *buffer = arena_alloc(arena, s.size);
  for (usize i = 0; i < s.size; ++i) {
    usize bit_count = u8_leading_ones((u8)s.data[i]);
    if (bit_count == 0) {
      buffer[i] = c_to_lower(s.data[i]);
    } else {
      memcpy(&buffer[i], &s.data[i], bit_count);
      i += bit_count - 1;
    }
  }
  return (Utf8){.len = s.len, .size = s.size, .data = buffer};
}

bool utf8_validate_bytes(Bytes bytes) {
  for (usize i = 0; i < bytes.size; i++) {
    usize bit_count = u8_leading_ones(bytes.data[i]);
    cebus_assert_return(bit_count <= 4, false);
    usize idx = i;
    for (usize j = 1; j < bit_count; j++) {
      cebus_assert_return(u8_leading_ones(bytes.data[idx + j]) == 1, false);
      i++;
    }
  }
  return true;
}

bool utf8_validate(Utf8 s) {
  return utf8_validate_bytes(bytes_from_parts(s.size, (const u8 *)s.data));
}
