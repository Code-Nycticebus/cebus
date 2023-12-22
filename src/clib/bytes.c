#include "bytes.h"
#include <stdio.h>
#include <string.h>

Bytes bytes_from_parts(size_t size, const uint8_t *data) {
  return (Bytes){.size = size, .data = data};
}

Bytes bytes_copy(Bytes bytes, Arena *arena) {
  uint8_t *buffer = arena_alloc(arena, bytes.size);
  memcpy(buffer, bytes.data, bytes.size);
  return bytes_from_parts(bytes.size, buffer);
}

bool bytes_eq(Bytes b1, Bytes b2) {
  if (b1.size != b2.size) {
    return false;
  }
  return memcmp(b1.data, b2.data, b1.size) == 0;
}

Str bytes_hex(Bytes bytes, Arena *arena) {
  char *buffer = arena_calloc(arena, bytes.size * 2 + 1);
  size_t b_idx = 0;
  for (size_t i = 0; i < bytes.size; i++) {
    b_idx += snprintf(&buffer[b_idx], 3, "%02x", bytes.data[i]);
  }
  return str_from_parts(b_idx, buffer);
}
