#include "utf8.h"

#include <string.h>

#include "clib/arena.h"
#include "datatypes/bytes.h"

#define LEADING_TWO_BITS ((uint8_t)0xC0)
#define LEADING_FIRST_BIT ((uint8_t)0x80)

Utf8 utf8_decode(Bytes bytes) {
  size_t len = 0;
  for (size_t i = 0; i < bytes.size; i++) {
    if ((bytes.data[i] & LEADING_TWO_BITS) != LEADING_FIRST_BIT) {
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
  size_t i = 0;
  size_t len = 0;
  for (; i < str->size && len < 2; i++) {
    if ((str->data[i] & LEADING_TWO_BITS) != LEADING_FIRST_BIT) {
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
