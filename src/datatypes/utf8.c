#include "utf8.h"

#include <stdio.h>
#include <string.h>

#include "clib/arena.h"
#include "datatypes/bytes.h"

Utf8 utf8_decode(Bytes bytes) {
  const uint8_t lead_two = 0xC0;
  const uint8_t lead_first = 0x80;
  size_t len = 0;
  for (size_t i = 0; i < bytes.size; i++) {
    if ((bytes.data[i] & lead_two) != lead_first) {
      len++;
    }
  }
  return (Utf8){.len = len, .size = bytes.size, .data = (char *)bytes.data};
}

Bytes utf8_encode(Utf8 s) {
  return bytes_from_parts(s.size, (uint8_t *)s.data);
}

Utf8 utf8_next(Utf8 *str) {
  if (str->len < 2) {
    Utf8 res = *str;
    *str = (Utf8){0};
    return res;
  }
  const uint8_t lead_two = 0xC0;
  const uint8_t lead_first = 0x80;
  size_t i = 0;
  size_t len = 0;
  for (; i < str->size && len < 2; i++) {
    if ((str->data[i] & lead_two) != lead_first) {
      len++;
    }
  }
  Utf8 res = (Utf8){.len = 1, .size = i - 1, .data = str->data};
  str->size -= i - 1;
  str->len--;
  str->data += i - 1;
  return res;
}

Utf8 utf8_copy(Utf8 str, Arena *arena) {
  char *buffer = arena_calloc(arena, str.size + 1);
  memcpy(buffer, str.data, str.size);
  return (Utf8){.len = str.len, .size = str.size, .data = buffer};
}
