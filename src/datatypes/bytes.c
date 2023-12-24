#include "bytes.h"
#include <stdio.h>
#include <string.h>

#define FIRST_BIT ((u8)0x80)

Bytes bytes_from_parts(usize size, const u8 *data) {
  return (Bytes){.size = size, .data = data};
}

Bytes bytes_copy(Bytes bytes, Arena *arena) {
  u8 *buffer = arena_alloc(arena, bytes.size);
  memcpy(buffer, bytes.data, bytes.size);
  return bytes_from_parts(bytes.size, buffer);
}

bool bytes_eq(Bytes b1, Bytes b2) {
  if (b1.size != b2.size) {
    return false;
  }
  return memcmp(b1.data, b2.data, b1.size) == 0;
}

Bytes bytes_slice(Bytes bytes, usize idx1, usize idx2) {
  if (idx2 <= idx1 || bytes.size <= idx1 || bytes.size < idx2) {
    return BYTES_STR("");
  }
  return bytes_from_parts(idx2 - idx1, &bytes.data[idx1]);
}

Bytes bytes_take(Bytes *bytes, usize count) {
  if (bytes->size < count) {
    count = bytes->size;
  }
  Bytes ret = bytes_from_parts(count, bytes->data);
  bytes->size -= count;
  bytes->data += count;
  return ret;
}

Str bytes_hex(Bytes bytes, Arena *arena) {
  char *buffer = arena_calloc(arena, bytes.size * 2 + 1);
  usize b_idx = 0;
  for (usize i = 0; i < bytes.size; i++) {
    if (i == 0) {
      b_idx += snprintf(&buffer[b_idx], 3, "%x", bytes.data[i]);
    } else {
      b_idx += snprintf(&buffer[b_idx], 3, "%02x", bytes.data[i]);
    }
  }
  return (Str){.len = b_idx, .data = buffer};
}
