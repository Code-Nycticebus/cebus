#include "utf8.h"

#include <string.h>

#include "clib/arena.h"
#include "clib/asserts.h"
#include "datatypes/bytes.h"

bool utf8_validate_bytes(Bytes bytes) {
  for (size_t i = 0; i < bytes.size; i++) {
    size_t bit_count = bytes_leading_ones(bytes.data[i]);
    clib_assert_return(bit_count <= 4, false);
    size_t idx = i;
    for (size_t j = 1; j < bit_count; j++) {
      clib_assert_return(bytes_leading_ones(bytes.data[idx + j]) == 1, false);
      i++;
    }
  }
  return true;
}

bool utf8_validate(Utf8 s) {
  return utf8_validate_bytes(bytes_from_parts(s.size, (uint8_t *)s.data));
}

bool utf8_try_decode(Bytes bytes, Utf8 *out) {
  size_t len = 0;
  for (size_t i = 0; i < bytes.size; i++) {
    size_t bit_count = bytes_leading_ones(bytes.data[i]);
    clib_assert_return(bit_count <= 4, false);
    size_t idx = i;
    for (size_t j = 1; j < bit_count; j++) {
      clib_assert_return(bytes_leading_ones(bytes.data[idx + j]) == 1, false);
      i++;
    }
    len++;
  }
  out->len = len;
  out->size = bytes.size;
  out->data = (char *)bytes.data;
  return true;
}

Utf8 utf8_decode(Bytes bytes) {
  Utf8 str = {0};
  bool ret = utf8_try_decode(bytes, &str);
  clib_assert(ret, "Decoding Utf-8 failed");
  return str;
}

bool utf8_try_encode(Utf8 s, Bytes *out) {
  bool ret = utf8_validate(s);
  clib_assert_return(ret, false);
  *out = bytes_from_parts(s.size, (uint8_t *)s.data);
  return true;
}

Bytes utf8_encode(Utf8 s) {
  bool ret = utf8_validate(s);
  clib_assert(ret, "Encoding Utf8 failed");
  return bytes_from_parts(s.size, (uint8_t *)s.data);
}

bool utf8_eq(Utf8 s1, Utf8 s2) {
  if (s1.len != s2.len || s1.size != s2.size) {
    return false;
  }
  return memcmp(s1.data, s2.data, s1.size) == 0;
}

bool utf8_try_next(Utf8 *str, Utf8 *out) {
  size_t bit_count = bytes_leading_ones(str->data[0]);
  clib_assert_return(bit_count <= 4, false);
  clib_assert_return(bit_count != 1, false);
  size_t bytes = bit_count == 0 ? 1 : bit_count;
  clib_assert_return(bytes <= str->size, false);
  *out = (Utf8){.len = 1, .size = bytes, .data = str->data};
  str->size -= bytes;
  str->len--;
  str->data += bytes;
  return true;
}

Utf8 utf8_next(Utf8 *str) {
  Utf8 res = {0};
  bool ret = utf8_try_next(str, &res);
  clib_assert(ret, "Getting next character failed");
  return res;
}

Utf8 utf8_copy(Utf8 str, Arena *arena) {
  char *buffer = arena_calloc(arena, str.size + 1);
  memcpy(buffer, str.data, str.size);
  return (Utf8){.len = str.len, .size = str.size, .data = buffer};
}
